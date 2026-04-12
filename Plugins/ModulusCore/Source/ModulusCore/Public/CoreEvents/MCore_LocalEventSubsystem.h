// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_LocalEventSubsystem.h
 *
 * Per-LocalPlayer subsystem for broadcasting and receiving client-only
 * GameplayTag events with zero network overhead.
 */

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "MCore_LocalEventSubsystem.generated.h"

class UMCore_EventListenerComp;
struct FMCore_EventData;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLocalEventBroadcast, const FMCore_EventData& /*EventData*/);

/**
 * Local event subsystem for client-only, non-networked GameplayTag events.
 * Each LocalPlayer has its own instance (split-screen safe).
 *
 * Events broadcast here never reach other clients or the server.
 * Use UMCore_GlobalEventSubsystem for networked events.
 */
UCLASS()
class MODULUSCORE_API UMCore_LocalEventSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	/** Register listener component. Called automatically by UMCore_EventListenerComp::BeginPlay() */
	void RegisterLocalListener(UMCore_EventListenerComp* ListenerComponent);

	/** Unregister listener component. Called automatically by UMCore_EventListenerComp::EndPlay() */
	void UnregisterLocalListener(UMCore_EventListenerComp* ListenerComponent);

	/**
	 * Broadcast event to all registered local listeners.
	 *
	 * Use UMCore_EventFunctionLibrary::BroadcastLocalEvent() instead of calling this directly.
	 */
	void BroadcastLocalEvent(const FMCore_EventData& EventData);

	/** Native delegate fired on every local event broadcast. Subsystems can bind here instead of using EventListenerComp. */
	FOnLocalEventBroadcast OnLocalEventBroadcast;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:
	/* Registered local listener components */
	UPROPERTY()
	TArray<TWeakObjectPtr<UMCore_EventListenerComp>> LocalListeners;
};
