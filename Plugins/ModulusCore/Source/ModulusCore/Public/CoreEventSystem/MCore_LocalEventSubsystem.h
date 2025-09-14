// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "MCore_LocalEventSubsystem.generated.h"

class UMCore_EventListenerComp;
struct FMCore_EventData;

/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_LocalEventSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Register a listener component for local events
	 */
	void RegisterLocalListener(UMCore_EventListenerComp* ListenerComponent);

	/**
	 * Unregister a listener component from local events
	 */
	void UnregisterLocalListener(UMCore_EventListenerComp* ListenerComponent);

	/**
	 * Broadcast local event to registered listeners on this client only
	 */
	void BroadcastLocalEvent(const FMCore_EventData& EventData);

private:
	// Local listener components on this client
	UPROPERTY()
	TArray<TWeakObjectPtr<UMCore_EventListenerComp>> LocalListeners;

	// Clean up stale local listener references
	void CleanupStaleLocalListeners();
};
