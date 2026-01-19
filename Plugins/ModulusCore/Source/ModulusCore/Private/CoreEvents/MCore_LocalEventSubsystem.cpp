// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreEvents/MCore_LocalEventSubsystem.h"
#include "CoreData/Logging/LogModulusEvent.h"
#include "CoreData/DevSettings/MCore_CoreSettings.h"
#include "CoreEvents/MCore_EventListenerComp.h"
#include "CoreData/Types/Events/MCore_EventData.h"

/** Conditional verbose logging macro - only logs when Event System Logging is enabled in Project Settings */
#define MCORE_EVENT_LOG(Format, ...) \
	do { \
		if (const UMCore_CoreSettings* Settings = UMCore_CoreSettings::Get()) \
		{ \
			if (Settings->IsEventLoggingEnabled()) \
			{ \
				UE_LOG(LogModulusEvent, Log, Format, ##__VA_ARGS__); \
			} \
		} \
	} while(0)

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
		MCORE_EVENT_LOG(TEXT("Registered local event listener: %s"),
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
		MCORE_EVENT_LOG(TEXT("Unregistered local event listener: %s"),
			ListenerComponent ? *ListenerComponent->GetName() : TEXT("Unknown"));
	}
}

void UMCore_LocalEventSubsystem::BroadcastLocalEvent(const FMCore_EventData& EventData)
{
	if (LocalListeners.Num() == 0 || !EventData.IsValid()) { return; }

	MCORE_EVENT_LOG(TEXT("Broadcasting local event: %s"),
		*EventData.EventTag.ToString());
	
	for (int32 i = LocalListeners.Num() - 1; i >= 0; --i)
	{
		TWeakObjectPtr<UMCore_EventListenerComp>& WeakListener = LocalListeners[i];

		
		if (WeakListener.IsValid())
		{
			UMCore_EventListenerComp* CurrentListener = WeakListener.Get();
		
			if (CurrentListener->ShouldReceiveEvent(EventData, false))
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
