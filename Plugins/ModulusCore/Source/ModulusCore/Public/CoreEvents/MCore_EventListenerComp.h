// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CoreNetworking/MCore_NetworkingComponent.h"
#include "MCore_EventListenerComp.generated.h"

class UMCore_LocalEventSubsystem;
class UMCore_GlobalEventSubsystem;
struct FMCore_EventData;

/**
 * Drop-in component for actors that need to receive events
 *
 * Handles automatic registration with Local and Global event subsystems.
 * Filters events by GameplayTag subscriptions. Add to any actor to receive event notifications.
 *
 * Common Uses:
 * - UI widgets listening for game state changes (quest updates, player stats)
 * - Gameplay actors responding to world events (time of day, weather changes)
 * - Cross-plugin communication without direct dependencies
 *
 * Blueprint Usage:
 * 1. Add component to actor/widget
 * 2. Set SubscribedEvents tags (or leave empty to receive all events)
 * 3. Implement OnEventReceived Blueprint event
 */
UCLASS(ClassGroup=(ModulusGameFramework), BlueprintType, meta=(BlueprintSpawnableComponent))
class MODULUSCORE_API UMCore_EventListenerComp : public UMCore_NetworkingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMCore_EventListenerComp();

	/** Tags to filter events (e.g., MCore.Events.Player.*, MCore.Events.Quest.Completed). Leave empty to receive all events */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Listening", meta = (Categories = "MCore.Events"))
	FGameplayTagContainer SubscribedEvents;

	/** Receive events broadcast locally (this client only) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Listening")
	bool bReceiveLocalEvents{true};

	/** Receive events broadcast globally (networked from server) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Listening")
	bool bReceiveGlobalEvents{true};

	/**
	 * Called when a subscribed event is received
	 *
	 * Implement in Blueprint to handle event notifications.
	 * Use EventData.EventTag to determine event type, then query EventData.Parameters or subsystems for details.
	 *
	 * @param EventData - Event information (tag, parameters, timestamp)
	 * @param bWasGlobalEvent - True if event came from Global subsystem (networked), false if Local
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Event Handling")
	void OnEventReceived(const FMCore_EventData& EventData, bool bWasGlobalEvent);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/** Internal - called by subsystems to deliver events. Do not call directly */
	void DeliverEvent(const FMCore_EventData& EventData, bool bWasGlobalEvent);

	/** Internal - check if this component should receive a specific event based on filters */
	bool ShouldReceiveEvent(const FMCore_EventData& EventData, bool bIsGlobalEvent) const;

private:
	// Cached references to both subsystems
	UPROPERTY()
	TWeakObjectPtr<UMCore_LocalEventSubsystem> CachedLocalSubsystem;
    
	UPROPERTY()
	TWeakObjectPtr<UMCore_GlobalEventSubsystem> CachedGlobalSubsystem;

	bool bRegisteredWithSubsystem{false};
};
