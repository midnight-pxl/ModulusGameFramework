// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreEventSystem/MCore_LocalEventSubsystem.h"

#include "CoreData/CoreLogging/LogModulusEvent.h"
#include "CoreEventSystem/MCore_EventListenerComp.h"
#include "CoreEventSystem/MCore_EventData.h"
#include "DSP/BufferDiagnostics.h"


void UMCore_LocalEventSubsystem::RegisterLocalListener(UMCore_EventListenerComp* ListenerComponent)
{
	if (IsValid(ListenerComponent))
	{
		LocalListeners.AddUnique(ListenerComponent);
		UE_LOG(LogModulusEvent, Verbose, TEXT("Registered local event listener: %s"),
			*ListenerComponent->GetName());
	}
}

void UMCore_LocalEventSubsystem::UnregisterLocalListener(UMCore_EventListenerComp* ListenerComponent)
{
	int32 RemovedCount = LocalListeners.RemoveAll([ListenerComponent](const TWeakObjectPtr<UMCore_EventListenerComp>& WeakListener)
	{
		return !WeakListener.IsValid() || WeakListener.Get() == ListenerComponent;
	});
	
	if (RemovedCount > 0)
	{
		UE_LOG(LogModulusEvent, Verbose, TEXT("Unregistered local event listener: %s"),
			ListenerComponent ? *ListenerComponent->GetName() : TEXT("Unknown"));
	}
}

void UMCore_LocalEventSubsystem::BroadcastLocalEvent(const FMCore_EventData& EventData)
{
	// Early exit if no listeners or event invalid
	if (LocalListeners.Num() == 0 || !EventData.IsValid()) { return; }

	UE_LOG(LogModulusEvent, Verbose, TEXT("Broadcasted local event: %s"),
		*EventData.EventTag.ToString());

	static int32 CleanupCounter = 0;
	if (++CleanupCounter % 100 == 0) { CleanupStaleLocalListeners(); }
	
	for (const TWeakObjectPtr<UMCore_EventListenerComp>& WeakListener : LocalListeners)
	{
		if (UMCore_EventListenerComp* CurrentListener = WeakListener.Get())
		{
			if (CurrentListener->ShouldReceiveEvent(EventData, false)) // false == not global event
			{
				CurrentListener->DeliverEvent(EventData, false);
			}
		}
	}
}

void UMCore_LocalEventSubsystem::CleanupStaleLocalListeners()
{
	int32 InitialCount = LocalListeners.Num();
	LocalListeners.RemoveAll([](const TWeakObjectPtr<UMCore_EventListenerComp>& WeakListener)
	{
		return !WeakListener.IsValid();
	});

	int32 RemovedCount = InitialCount - LocalListeners.Num();
	if (RemovedCount > 0)
	{
		UE_LOG(LogModulusEvent, Log, TEXT("Cleaned up %d stale local listeners"),
			RemovedCount);
	}
}
