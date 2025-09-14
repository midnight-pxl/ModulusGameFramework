// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CoreNetworkingSystem/MCore_NetworkingComponent.h"
#include "MCore_EventListenerComp.generated.h"

class UMCore_LocalEventSubsystem;
class UMCore_GlobalEventSubsystem;
struct FMCore_EventData;

/**
 * Drop-in component for actors that need to receive events
 * Handles automatic registration/cleanup and tag-based filtering
 */
UCLASS(ClassGroup=(ModulusGameFramework), BlueprintType, meta=(BlueprintSpawnableComponent))
class MODULUSCORE_API UMCore_EventListenerComp : public UMCore_NetworkingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMCore_EventListenerComp();

	/**
	 * Events to listen for (using Gameplay Tags)
	 * Leave empty to receive all events
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Listening", meta = (Categories = "MCore.Events"))
	FGameplayTagContainer SubscribedEvents;

	/**
	 * Whether to receive local events (this client only)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Listening")
	bool bReceiveLocalEvents{true};

	/**
	 * Whether to receive global events (from all players)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Listening")
	bool bReceiveGlobalEvents{true};
	
	/**
	 * Called when a subscribed event is received
	 * Implement this in Blueprint to handle events
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Event Handling")
	void OnEventReceived(const FMCore_EventData& EventData, bool bWasGlobalEvent);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/**
	 * Internal - called by subsystems to deliver events
	 */
	void DeliverEvent(const FMCore_EventData& EventData, bool bWasGlobalEvent);

	/**
	 * Check if this component should receive a specific event
	 */
	bool ShouldReceiveEvent(const FMCore_EventData& EventData, bool bIsGlobalEvent) const;

private:
	// Cached references to both subsystems
	UPROPERTY()
	TWeakObjectPtr<UMCore_LocalEventSubsystem> CachedLocalSubsystem;
    
	UPROPERTY()
	TWeakObjectPtr<UMCore_GlobalEventSubsystem> CachedGlobalSubsystem;

	bool bRegisteredWithSubsystem{false};
};
