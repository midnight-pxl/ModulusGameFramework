// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "MCore_LocalEventSubsystem.generated.h"

class UMCore_EventListenerComp;
struct FMCore_EventData;

/**
 * Local event subsystem for client-only events
 *
 * Manages event broadcasting and listener registration for events that don't need network synchronization.
 * Each LocalPlayer has its own instance (supports split-screen).
 *
 * Common Uses:
 * - UI updates (menu opened, button clicked)
 * - Client-side visual effects
 * - Local player notifications
 *
 * Network: Client-local only
 * - Events broadcast here never reach other clients or the server
 * - Use UMCore_GlobalEventSubsystem for networked events
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
	 * Broadcast event to all registered local listeners
	 *
	 * Use UMCore_EventFunctionLibrary::BroadcastLocalEvent() instead of calling this directly.
	 *
	 * @param EventData - Event to broadcast (tag, parameters, timestamp)
	 */
	void BroadcastLocalEvent(const FMCore_EventData& EventData);
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:
	/** Registered local listener components */
	UPROPERTY()
	TArray<TWeakObjectPtr<UMCore_EventListenerComp>> LocalListeners;
};
