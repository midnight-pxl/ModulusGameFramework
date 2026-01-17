// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreEvents/MCore_LocalEventSubsystem.h"
#include "CoreData/Logging/LogModulusEvent.h"
#include "CoreEvents/MCore_EventListenerComp.h"
#include "CoreData/Types/Events/MCore_EventData.h"

void UMCore_LocalEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UMCore_LocalEventSubsystem::Deinitialize()
{
	LocalListeners.Empty();
	
	Super::Deinitialize();
}

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
	int32 RemovedCount = LocalListeners.RemoveAll([ListenerComponent](
		const TWeakObjectPtr<UMCore_EventListenerComp>& WeakListener)
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

	UE_LOG(LogModulusEvent, Verbose, TEXT("Broadcasting local event: %s"),
		*EventData.EventTag.ToString());
	
	for (int32 i = LocalListeners.Num() - 1; i >= 0; --i)
	{
		TWeakObjectPtr<UMCore_EventListenerComp>& WeakListener = LocalListeners[i];

		
		if (WeakListener.IsValid())
		{
			UMCore_EventListenerComp* CurrentListener = WeakListener.Get();
		
			if (CurrentListener->ShouldReceiveEvent(EventData, false)) // false == not global event
			{
				CurrentListener->DeliverEvent(EventData, false);
			}
		}
		else
		{
			LocalListeners.RemoveAtSwap(i);
		}
	}
}
