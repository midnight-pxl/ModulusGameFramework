// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreEvents/MCore_EventListenerComp.h"
#include "CoreData/Logging/LogModulusEvent.h"
#include "CoreData/Types/Events/MCore_EventData.h"
#include "CoreEvents/MCore_GlobalEventSubsystem.h"
#include "CoreEvents/MCore_LocalEventSubsystem.h"

UMCore_EventListenerComp::UMCore_EventListenerComp()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = false;

	bReceiveLocalEvents = true;
	bReceiveGlobalEvents = true;
}

void UMCore_EventListenerComp::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("EventListener BeginPlay: No valid world"));
		return;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("EventListener BeginPlay: No valid game instance"));
		return;
	}

	/** Register with local event subsystem */
	if (bReceiveLocalEvents)
	{
		if (ULocalPlayer* LocalPlayer = GameInstance->GetFirstGamePlayer())
		{
			if (UMCore_LocalEventSubsystem* LocalEventSys = LocalPlayer->GetSubsystem<UMCore_LocalEventSubsystem>())
			{
				CachedLocalSubsystem = LocalEventSys;
				LocalEventSys->RegisterLocalListener(this);
			}
			else
			{
				UE_LOG(LogModulusEvent, Warning, TEXT("Failed to get LocalEventSubsystem for %s"), *GetName());
			}
		}
	}

	/** Register with global event subsystem */
	if (bReceiveGlobalEvents)
	{
		if (UMCore_GlobalEventSubsystem* GlobalEventSys = GameInstance->GetSubsystem<UMCore_GlobalEventSubsystem>())
		{
			CachedGlobalSubsystem = GlobalEventSys;
			GlobalEventSys->RegisterGlobalListener(this);
		}
		else
		{
			UE_LOG(LogModulusEvent, Warning, TEXT("Failed to get GlobalEventSubsystem for %s"), *GetName());
		}
	}

	UE_LOG(LogModulusEvent, Verbose, TEXT("EventListener initialized: %s (Local: %s, Global: %s)"), 
		   *GetName(), 
		   bReceiveLocalEvents ? TEXT("Yes") : TEXT("No"),
		   bReceiveGlobalEvents ? TEXT("Yes") : TEXT("No"));
}

void UMCore_EventListenerComp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	/** Unregister from both subsystems */
	if (UMCore_LocalEventSubsystem* LocalEventSys = CachedLocalSubsystem.Get())
	{
		LocalEventSys->UnregisterLocalListener(this);
	}
	CachedLocalSubsystem.Reset();

	if (UMCore_GlobalEventSubsystem* GlobalEventSys = CachedGlobalSubsystem.Get())
	{
		GlobalEventSys->UnregisterGlobalListener(this);
	}
	CachedGlobalSubsystem.Reset();

	UE_LOG(LogModulusEvent, Verbose, TEXT("EventListener cleanup: %s"), *GetName());

	Super::EndPlay(EndPlayReason);
}

void UMCore_EventListenerComp::DeliverEvent(const FMCore_EventData& EventData, bool bWasGlobalEvent)
{
	UE_LOG(LogModulusEvent, VeryVerbose, TEXT("Delivering event to %s: %s (Global: %s)"),
	   *GetName(), *EventData.EventTag.ToString(), bWasGlobalEvent ? TEXT("Yes") : TEXT("No"));

	OnEventReceived(EventData, bWasGlobalEvent);
}

bool UMCore_EventListenerComp::ShouldReceiveEvent(const FMCore_EventData& EventData, bool bIsGlobalEvent) const
{
	if (bIsGlobalEvent && !bReceiveGlobalEvents) { return false; }
	if (!bIsGlobalEvent && !bReceiveLocalEvents) { return false; }

	/** Empty subscription list means receive all events */
	if (!SubscribedEvents.IsEmpty())
	{
		return SubscribedEvents.HasTag(EventData.EventTag);
	}

	return true;
}
