// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_GlobalEventSubsystem.h
 *
 * GameInstance subsystem for server-authoritative event broadcasting
 * across all connected clients via the GlobalEventReplicator.
 */

#pragma once

#include "CoreMinimal.h"
#include "CoreData/Types/Events/MCore_EventData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MCore_GlobalEventSubsystem.generated.h"

class UMCore_EventListenerComp;
class UMCore_GlobalEventReplicator;

/**
 * Server-authoritative event subsystem for networked GameplayTag events.
 * One instance per GameInstance (shared by all players).
 *
 * Requires UMCore_GlobalEventReplicator on GameState for network transport.
 * Without a replicator, events broadcast locally only with a warning.
 */
UCLASS(Config=ModulusCore)
class MODULUSCORE_API UMCore_GlobalEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Broadcast global event to all clients
	 *
	 * With Replicator (networked):
	 * - Server: Delivers locally + multicasts to all clients
	 * - Client: Sends server RPC for validation and broadcast
	 *
	 * Without Replicator (standalone or not configured):
	 * - Server/Standalone: Delivers to local listeners only
	 * - Client: Logs warning, event not broadcast
	 *
	 * Use UMCore_EventFunctionLibrary::BroadcastGlobalEvent() instead of calling this directly.
	 */
	void BroadcastGlobalEvent(const FMCore_EventData& EventData);
	
	/**
	 * Register the network replicator component.
	 * Called by GlobalEventReplicator::BeginPlay().
	 */
	void RegisterEventReplicator(UMCore_GlobalEventReplicator* Replicator);

	/**
	 * Unregister the network replicator component.
	 * Called by GlobalEventReplicator::EndPlay().
	 */
	void UnregisterEventReplicator(UMCore_GlobalEventReplicator* Replicator);

	/** 
	 * Register listener for global events.
	 * Called by UMCore_EventListenerComp::BeginPlay()
	 */
	void RegisterGlobalListener(UMCore_EventListenerComp* ListenerComponent);

	/**
	 * Unregister listener from global events.
	 * Called by UMCore_EventListenerComp::EndPlay()
	 */
	void UnregisterGlobalListener(UMCore_EventListenerComp* ListenerComponent);
	
	/**
	 * Deliver event to all registered local listeners.
	 * Called by GlobalEventReplicator after network transport.
	 */
	void DeliverToLocalListeners(const FMCore_EventData& EventData);

	/** Returns true if this instance has authority to broadcast global events (server or standalone). */
	UFUNCTION(BlueprintCallable, Category = "Event System")
	bool HasGlobalEventAuthority() const;
	
	/** Validates an inbound event request. Override to add custom validation rules. */
	bool ValidateEventRequest(const FMCore_EventData& EventData) const;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	/* Registered global listener components */
	UPROPERTY()
	TArray<TWeakObjectPtr<UMCore_EventListenerComp>> GlobalListeners;
	
	/* Cached reference to the network replicator on GameState */
	TWeakObjectPtr<UMCore_GlobalEventReplicator> EventReplicator;

	/* Check if we're in a networked game (not standalone) */
	bool IsNetworkedGame() const;
};