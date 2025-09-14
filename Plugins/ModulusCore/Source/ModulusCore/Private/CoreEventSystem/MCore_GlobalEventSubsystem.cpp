// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreEventSystem/MCore_GlobalEventSubsystem.h"

#include "CoreData/CoreLogging/LogModulusEvent.h"
#include "CoreEventSystem/MCore_EventData.h"
#include "CoreEventSystem/MCore_EventListenerComp.h"

void UMCore_GlobalEventSubsystem::BroadcastGlobalEvent(const FMCore_EventData& EventData)
{
	if (!EventData.IsValid())
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("Attempted to broadcast invalid global event"));
		return;
	}

	if (HasGlobalEventAuthority())
	{
		UE_LOG(LogModulusEvent, Verbose, TEXT("Broadcasting global event (Server): %s"), 
			  *EventData.EventTag.ToString());

		// Add to history if true
		if (bSendEventHistoryToLateJoiners)
		{
			EventHistory.Add(EventData);

			if (EventHistory.Num() > MaxEventHistorySize)
			{
				int32 RemoveCount = EventHistory.Num() - MaxEventHistorySize;
				EventHistory.RemoveAt(0, RemoveCount);
				UE_LOG(LogModulusEvent, Verbose, TEXT("Trimmed event history, removed %d old events"),
					RemoveCount);
			}
		}

		// Broadcast to all clients
		MulticastGlobalEvent(EventData);
	}
	else
	{
		// Client receiving multicast - deliver locally
		UE_LOG(LogModulusEvent, VeryVerbose, TEXT("Receiving global event (Client): %s"), 
			   *EventData.EventTag.ToString());
		
		DeliverGlobalEventToLocalListeners(EventData);
	}
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

void UMCore_GlobalEventSubsystem::OnClientConnected(APlayerController* NewPlayer)
{
	if (!IsValid(NewPlayer) || !bSendEventHistoryToLateJoiners || EventHistory.Num() == 0) { return; }

	UE_LOG(LogModulusEvent, Log, TEXT("Sending %d historical events to late-joining player: %s"), 
		   EventHistory.Num(), *NewPlayer->GetName());

	// Send historical events to the new client
	for (const FMCore_EventData& HistoricalEvent : EventHistory)
	{
		ClientReceiveEventHistory(HistoricalEvent);
	}
}

bool UMCore_GlobalEventSubsystem::ServerBroadcastGlobalEvent_Validate(const FMCore_EventData& EventData)
{
	if (!EventData.IsValid())
	{
		switch (ValidationStrictness)
		{
		case EMCore_ValidationStrictness::Permissive:
			UE_LOG(LogModulusEvent, Warning, TEXT("Invalid event data received, ignoring"));
			return false;
		case EMCore_ValidationStrictness::Balanced:
			UE_LOG(LogModulusEvent, Error, TEXT("Invalid event data: %s"), *EventData.EventTag.ToString());
			return false;
		case EMCore_ValidationStrictness::Strict:
			UE_LOG(LogModulusEvent, Fatal, TEXT("Critical validation failure: Invalid event data"));
			return false;
		}
	}
    
	// Additional validation based on strictness
	if (ValidationStrictness == EMCore_ValidationStrictness::Strict)
	{
		// Strict mode: Validate parameter count, tag hierarchy, etc.
		if (EventData.EventParams.Num() > 50) // Prevent parameter spam
		{
			UE_LOG(LogModulusEvent, Error, TEXT("Event rejected: Too many parameters (%d > 50)"),
				EventData.EventParams.Num());
			return false;
		}

		if (!EventData.EventTag.ToString().StartsWith(TEXT("MCore.Events.")))
		{
			UE_LOG(LogModulusEvent, Error, TEXT("Event rejected: Invalid tag format in strict mode: %s"), 
				   *EventData.EventTag.ToString());
			return false;
		}
	}
    
	return true;
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
	BroadcastGlobalEvent(EventData);
}

void UMCore_GlobalEventSubsystem::ClientReceiveEventHistory_Implementation(const FMCore_EventData& HistoricalEvent)
{
	UE_LOG(LogModulusEvent, Verbose, TEXT("Received historical event: %s"), 
	   *HistoricalEvent.EventTag.ToString());
    
	// Deliver historic event to local listeners
	DeliverGlobalEventToLocalListeners(HistoricalEvent);
}

void UMCore_GlobalEventSubsystem::DeliverGlobalEventToLocalListeners(const FMCore_EventData& EventData)
{
	// Performance: Clean up stale references periodically
	static int32 CleanupCounter = 0;
	if (++CleanupCounter % 100 == 0)
	{
		CleanupStaleGlobalListeners();
	}

	// Deliver to global listeners on this client
	for (const TWeakObjectPtr<UMCore_EventListenerComp>& WeakListener : GlobalListeners)
	{
		if (UMCore_EventListenerComp* Listener = WeakListener.Get())
		{
			if (Listener->ShouldReceiveEvent(EventData, true)) // true = global event
			{
				Listener->DeliverEvent(EventData, true);
			}
		}
	}
}

void UMCore_GlobalEventSubsystem::CleanupStaleGlobalListeners()
{
	int32 InitialCount = GlobalListeners.Num();
	GlobalListeners.RemoveAll([](const TWeakObjectPtr<UMCore_EventListenerComp>& WeakPtr)
	{
		return !WeakPtr.IsValid();
	});
    
	int32 RemovedCount = InitialCount - GlobalListeners.Num();
	if (RemovedCount > 0)
	{
		UE_LOG(LogModulusEvent, Log, TEXT("Cleaned up %d stale global listeners"), RemovedCount);
	}
}

bool UMCore_GlobalEventSubsystem::HasGlobalEventAuthority() const
{
	const UWorld* World = GetWorld();
	if (!World) { return false; }

	const ENetMode NetMode = World->GetNetMode();
    
	// Single player always has authority
	if (NetMode == NM_Standalone) { return true; }
    
	// For networked games, only server has global event authority
	if (NetMode == NM_DedicatedServer) { return true; }
    
	// Listen server: Must have valid game mode
	if (NetMode == NM_ListenServer) { return World->GetAuthGameMode() != nullptr; }
    
	// Clients never have global authority
	return false;
}