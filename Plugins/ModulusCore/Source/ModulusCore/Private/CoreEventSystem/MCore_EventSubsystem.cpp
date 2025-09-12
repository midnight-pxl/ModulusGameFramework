// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreEventSystem/MCore_EventSubsystem.h"
#include "CoreData/CoreLogging/LogModulusEvent.h"
#include "CoreEventSystem/MCore_EventListenerComponent.h"
#include "CoreEventSystem/EventSystemData/MCore_EventDetails.h"

TWeakObjectPtr<UMCore_EventSubsystem> UMCore_EventSubsystem::CachedSubsystem = nullptr;

void UMCore_EventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogModulusEvent, Log, TEXT("UMCore_EventSubsystem::Initialized"));

	// for static function get performance
	CachedSubsystem = this;
}

void UMCore_EventSubsystem::Deinitialize()
{
	EventListeners.Empty();

	CachedSubsystem = nullptr;

	UE_LOG(LogModulusEvent, Log, TEXT("UMCore_EventSubsystem::Deinitialized"));
	
	Super::Deinitialize();
}

void UMCore_EventSubsystem::BroadcastEvent(const FMCore_EventData& Event)
{
	if (!Event.IsValid())
	{
		UE_LOG(LogModulusEvent, Log, TEXT("Attempted to broadcast an invalid event"));
		return;
	}

	// Network Authority validation for global events
	if (Event.EventScope == EMCore_EventScope::Global && !HasBroadcastAuthority(Event))
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("No authority to broadcast Global event: %s"), *Event.EventTag.ToString());
		return;
	}

	UE_LOG(LogModulusEvent, Verbose, TEXT("Broadcasting event: %s (Scope: %s, Parameter: %s)"), 
		*Event.EventTag.ToString(), 
		Event.EventScope == EMCore_EventScope::Local ? TEXT("Local") : TEXT("Global"),
		*Event.EventParameter);

	// Broadcast to all registered listeners
	for (int32 i = EventListeners.Num() - 1; i >= 0; --i)
	{
		if (EventListeners[i].GetInterface())
		{
			// Call the interface method via Blueprint
			EventListeners[i].GetInterface()->Execute_HandleEvent(EventListeners[i].GetObject(), Event);
		}
		else
		{
			// Remove invalid listeners
			EventListeners.RemoveAtSwap(i);
		}
	}
}

void UMCore_EventSubsystem::BroadcastSimple(const FGameplayTag& EventTag, EMCore_EventScope Scope,
	const FString& Parameter)
{
}

void UMCore_EventSubsystem::BroadcastUIEvent(const FGameplayTag& EventTag, const FString& Parameter)
{
}

void UMCore_EventSubsystem::BroadcastPlayerEvent(const FGameplayTag& EventTag, const FString& Parameter)
{
}

void UMCore_EventSubsystem::BroadcastGameplayEvent(const FGameplayTag& EventTag, const FString& Parameter)
{
}

void UMCore_EventSubsystem::RegisterListener(TScriptInterface<IMCore_EventListeners> Listener)
{
}

void UMCore_EventSubsystem::UnregisterListener(TScriptInterface<IMCore_EventListeners> Listener)
{
}

UMCore_EventSubsystem* UMCore_EventSubsystem::Get(const UObject* WorldContext)
{
}

bool UMCore_EventSubsystem::HasBroadcastAuthority(const FMCore_EventData& Event) const
{
}
