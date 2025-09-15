// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreEventSystem/MCore_GlobalEventSubsystem.h"
#include "CoreData/CoreLogging/LogModulusEvent.h"
#include "CoreEventSystem/MCore_EventData.h"
#include "CoreEventSystem/MCore_EventListenerComp.h"
#include "GameFramework/GameModeBase.h"

void UMCore_GlobalEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (bSendEventHistoryToLateJoiners && MaxEventHistorySize > 0)
	{
		EventHistory.SetNum(MaxEventHistorySize);
		UE_LOG(LogModulusEvent, Log, TEXT("Event history initialized with size of %d"), MaxEventHistorySize);
	}
}

void UMCore_GlobalEventSubsystem::Deinitialize()
{
	GlobalListeners.Empty();
	EventHistory.Empty();
	EventHistoryIndex = 0;
	
	Super::Deinitialize();
}

void UMCore_GlobalEventSubsystem::BroadcastGlobalEvent(const FMCore_EventData& EventData)
{
	if (!EventData.IsValid())
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("Attempted to broadcast invalid global event"));
		return;
	}
	
	if (!HasGlobalEventAuthority()) {
		UE_LOG(LogModulusEvent, Warning, TEXT("Attempted to broadcast global event %s without server authority"),
			*EventData.EventTag.ToString());
		return;
	}

	// Add to event history - circular buffer
	if (bSendEventHistoryToLateJoiners && MaxEventHistorySize > 0) {
		EventHistory[EventHistoryIndex] = EventData;
		EventHistoryIndex = (EventHistoryIndex + 1) % MaxEventHistorySize;
	}

	UE_LOG(LogModulusEvent, Verbose, TEXT("Broadcasting global event: %s"), *EventData.EventTag.ToString());

	// Deliver to current listeners
	DeliverGlobalEventToLocalListeners(EventData);

	// Send to all clients
	MulticastGlobalEvent(EventData);
}

void UMCore_GlobalEventSubsystem::RegisterGlobalListener(UMCore_EventListenerComp* ListenerComponent)
{
	if (IsValid(ListenerComponent))
	{
		GlobalListeners.AddUnique(ListenerComponent);
		UE_LOG(LogModulusEvent, Verbose, TEXT("Registered global event listener: %s"), 
			   *ListenerComponent->GetName());
	}
}

void UMCore_GlobalEventSubsystem::UnregisterGlobalListener(UMCore_EventListenerComp* ListenerComponent)
{
	int32 RemoveCount = GlobalListeners.RemoveAll([ListenerComponent](const TWeakObjectPtr<UMCore_EventListenerComp>& WeakPtr)
	{
		return !WeakPtr.IsValid() || WeakPtr.Get() == ListenerComponent;
	});

	if (RemoveCount > 0)
	{
		UE_LOG(LogModulusEvent, Verbose, TEXT("Unregistered global event listener: %s"),
			ListenerComponent ? *ListenerComponent->GetName() : TEXT("Unknown"));
	}
}

bool UMCore_GlobalEventSubsystem::HasGlobalEventAuthority() const
{
	const UWorld* World = GetWorld();
	if (!World) { return false; }

	const ENetMode NetMode = World->GetNetMode();
	
	switch (NetMode) {
	case NM_Standalone:
	case NM_DedicatedServer:
		return true;
        
	case NM_ListenServer:
		return World->GetAuthGameMode() != nullptr;

	case NM_Client:
		return false;
        
	default:
		return false;
	}
}

void UMCore_GlobalEventSubsystem::SendEventHistoryToPlayer(APlayerController* TargetPlayer)
{
	if (!TargetPlayer)
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("SendEventHistoryToPlayer: Invalid TargetPlayer"));
		return;
	}

	if (!HasGlobalEventAuthority())
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("SendEventHistoryToPlayer: No server authority"));
		return;
	}

	if (!bSendEventHistoryToLateJoiners)
	{
		UE_LOG(LogModulusEvent, Log, TEXT("SendEventHistoryToPlayer: Event history disabled in configuration"));
		return;
	}

	// Build active history array from circular buffer
	TArray<FMCore_EventData> ActiveHistory;
	ActiveHistory.Reserve(MaxEventHistorySize);

	for (int32 i = 0; i < MaxEventHistorySize; ++i)
	{
		int32 Index = (EventHistoryIndex + i) % MaxEventHistorySize;
		if (EventHistory.IsValidIndex(Index) && EventHistory[Index].IsValid())
		{
			ActiveHistory.Add(EventHistory[Index]);
		}
	}

	if (ActiveHistory.Num() > 0)
	{
		// Send to specific client via RPC
		ClientReceiveEventHistory(ActiveHistory);
		UE_LOG(LogModulusEvent, Log, TEXT("Sent %d historical events to player %s"), 
			   ActiveHistory.Num(), *TargetPlayer->GetName());
	}
	else
	{
		UE_LOG(LogModulusEvent, Log, TEXT("No event history to send to player %s"), 
			   *TargetPlayer->GetName());
	}
}

bool UMCore_GlobalEventSubsystem::ServerBroadcastGlobalEvent_Validate(const FMCore_EventData& EventData)
{
	if (!EventData.IsValid()) { return false; }
	
	switch (ValidationStrictness) {
	case EMCore_ValidationStrictness::Permissive:
		return true;
		
	case EMCore_ValidationStrictness::Balanced:
		return EventData.EventParams.Num() < 10 && EventData.ContextID.Len() < 100;
		
	case EMCore_ValidationStrictness::Strict:
		return EventData.EventParams.Num() < 6 && EventData.ContextID.Len() < 60;

	default:
		return false;
	}
}

void UMCore_GlobalEventSubsystem::ServerBroadcastGlobalEvent_Implementation(const FMCore_EventData& EventData)
{
	// Server receives RPC, validates, and broadcasts to all clients
	if (HasGlobalEventAuthority())
	{
		UE_LOG(LogModulusEvent, Verbose, TEXT("Server received client event request: %s"),
			*EventData.EventTag.ToString());
		BroadcastGlobalEvent(EventData);
	}
	else
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("Server authority check failed for client event: %s"), 
			   *EventData.EventTag.ToString());
	}
}

void UMCore_GlobalEventSubsystem::MulticastGlobalEvent_Implementation(const FMCore_EventData& EventData)
{
	// All clients (including server) receive this and deliver locally
	DeliverGlobalEventToLocalListeners(EventData);
}

void UMCore_GlobalEventSubsystem::ClientReceiveEventHistory_Implementation(const TArray<FMCore_EventData>& HistoricalEvents)
{
	UE_LOG(LogModulusEvent, Log, TEXT("Receiving %d historical events"), 
	   HistoricalEvents.Num());
    
	// Late-joining client receives event history
	for (const FMCore_EventData& HistoryEvent : HistoricalEvents)
	{
		if (HistoryEvent.IsValid())
		{
			DeliverGlobalEventToLocalListeners(HistoryEvent);
		}
	}
}

void UMCore_GlobalEventSubsystem::DeliverGlobalEventToLocalListeners(const FMCore_EventData& EventData)
{
	// Early exit if no listeners
	if (GlobalListeners.Num() == 0) { return; }

	// Deliver event to all registered local listeners
	for (int32 i = GlobalListeners.Num() - 1; i >= 0; --i)
	{
		TWeakObjectPtr<UMCore_EventListenerComp>& WeakListener = GlobalListeners[i];
        
		if (WeakListener.IsValid())
		{
			UMCore_EventListenerComp* Listener = WeakListener.Get();
			if (Listener && Listener->ShouldReceiveEvent(EventData, true))
			{
				Listener->DeliverEvent(EventData, true);
			}
		}
		else
		{
			// Remove stale reference during iteration
			GlobalListeners.RemoveAtSwap(i);
		}
	}
}