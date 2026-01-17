// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreEvents/MCore_GlobalEventSubsystem.h"
#include "CoreData/Logging/LogModulusEvent.h"
#include "CoreData/DevSettings/MCore_CoreSettings.h"
#include "CoreData/Types/Events/MCore_EventData.h"
#include "CoreEvents/MCore_EventListenerComp.h"

// Conditional verbose logging macro - only logs when Event System Logging is enabled in Project Settings
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

void UMCore_GlobalEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UMCore_GlobalEventSubsystem::Deinitialize()
{
	GlobalListeners.Empty();
	
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

	MCORE_EVENT_LOG(TEXT("Broadcasting global event: %s"), *EventData.EventTag.ToString());

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
		MCORE_EVENT_LOG(TEXT("Registered global event listener: %s"),
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
		MCORE_EVENT_LOG(TEXT("Unregistered global event listener: %s"),
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
		MCORE_EVENT_LOG(TEXT("Server received client event request: %s"),
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