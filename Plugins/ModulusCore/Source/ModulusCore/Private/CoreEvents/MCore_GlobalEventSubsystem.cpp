// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreEvents/MCore_GlobalEventSubsystem.h"
#include "CoreEvents/MCore_GlobalEventReplicator.h"
#include "CoreData/Logging/LogModulusEvent.h"
#include "CoreData/Types/Events/MCore_EventData.h"
#include "CoreEvents/MCore_EventListenerComp.h"

void UMCore_GlobalEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogModulusEvent, Log, TEXT("Initializing global event subsystem"));
}

void UMCore_GlobalEventSubsystem::Deinitialize()
{
	GlobalListeners.Empty();
	EventReplicator.Reset();
	
	Super::Deinitialize();
}

void UMCore_GlobalEventSubsystem::BroadcastGlobalEvent(const FMCore_EventData& EventData)
{
	if (!EventData.IsValid())
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("Attempted to broadcast invalid global event"));
		return;
	}
	
	UE_LOG(LogModulusEvent, Verbose, TEXT("BroadcastGlobalEvent: %s"), *EventData.EventTag.ToString());
	
	/** Route through event replicator if available */
	if (UMCore_GlobalEventReplicator* Replicator = EventReplicator.Get())
	{
		Replicator->RequestBroadcast(EventData);
	}
	else
	{
		/** No replicator: not configured or standalone */
		if (HasGlobalEventAuthority())
		{
			/** Deliver locally */
			DeliverToLocalListeners(EventData);
			
			if (IsNetworkedGame())
			{
				UE_LOG(LogModulusEvent, Warning,
					TEXT("Global event delivered locally only - no replicator found; "
						 "add UMCore_GlobalEventReplicator to GameState for network support."));
				return;
			}
		}
		else
		{
			/** Client w/o replicator: cannot broadcast */
			UE_LOG(LogModulusEvent, Warning,
				TEXT("Global event not broadcast - not authority and no replicator found; "
					 "add UMCore_GlobalEventReplicator to GameState for network support."))
			return;
		}
	}

	UE_LOG(LogModulusEvent, Log, TEXT("Broadcasting global event: %s"), *EventData.EventTag.ToString());
}

void UMCore_GlobalEventSubsystem::RegisterEventReplicator(UMCore_GlobalEventReplicator* Replicator)
{
	if (IsValid(Replicator))
	{
		EventReplicator = Replicator;
		UE_LOG(LogModulusEvent, Log, TEXT("Registered global event replicator: %s"),
			Replicator->GetOwner() ? *Replicator->GetOwner()->GetName() : TEXT("Unknown"));
	}
}

void UMCore_GlobalEventSubsystem::UnregisterEventReplicator(UMCore_GlobalEventReplicator* Replicator)
{
	if (EventReplicator.Get() == Replicator)
	{
		EventReplicator.Reset();
		UE_LOG(LogModulusEvent, Log, TEXT("Unregistered global event replicator"));
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

void UMCore_GlobalEventSubsystem::DeliverToLocalListeners(const FMCore_EventData& EventData)
{
	if (GlobalListeners.Num() == 0) { return; }
	
	UE_LOG(LogModulusEvent, Verbose, TEXT("Delivering global event '%s' to %d listeners"),
		*EventData.EventTag.ToString(), GlobalListeners.Num());
	
	/** Reverse traverse to remove stale entries */
	for (int32 i = GlobalListeners.Num() - 1; i >= 0; --i)
	{
		TWeakObjectPtr<UMCore_EventListenerComp>& CurListener = GlobalListeners[i];
		
		if (CurListener.IsValid())
		{
			UMCore_EventListenerComp* ListenerComp = CurListener.Get();
			if (ListenerComp && CurListener->ShouldReceiveEvent(EventData, /*bIsGlobalEvent*/ true))
			{
				CurListener->DeliverEvent(EventData, /*bIsGlobalEvent*/ true);
			}
		}
		else
		{
			/** invalid pointer -- clean up stale pointer */
			GlobalListeners.RemoveAt(i);
		}
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

bool UMCore_GlobalEventSubsystem::ValidateEventRequest(const FMCore_EventData& EventData) const
{
	if (!EventData.IsValid())
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("ValidateEventRequest: Invalid event data"));
		return false;
	}
	
	switch (ValidationStrictness)
	{
	case EMCore_ValidationStrictness::Permissive:
		return true;
		
	case EMCore_ValidationStrictness::Balanced:
		if (EventData.EventParams.Num() >= 10)
		{
			UE_LOG(LogModulusEvent, Warning, TEXT("ValidateEventRequest: Too many params (%d >= 10)"),
				EventData.EventParams.Num());
			return false;
		}
		if (EventData.ContextID.Len() >= 100)
		{
			UE_LOG(LogModulusEvent, Warning, TEXT("ValidateEventRequest: ContextID too long (%d >= 100)"),
				EventData.ContextID.Len());
			return false;
		}
		return true;
		
	case EMCore_ValidationStrictness::Strict:
		if (EventData.EventParams.Num() >= 6)
		{
			UE_LOG(LogModulusEvent, Warning, TEXT("ValidateEventRequest: Too many params (%d >= 6) [Strict]"),
				EventData.EventParams.Num());
			return false;
		}
		if (EventData.ContextID.Len() >= 60)
		{
			UE_LOG(LogModulusEvent, Warning, TEXT("ValidateEventRequest: ContextID too long (%d >= 60) [Strict]"),
				EventData.ContextID.Len());
			return false;
		}
		return true;

	default:
		return false;
	}
}

bool UMCore_GlobalEventSubsystem::IsNetworkedGame() const
{
	const UWorld* World = GetWorld();
	if (!World) { return false; }
	
	return World->GetNetMode() != NM_Standalone;
}