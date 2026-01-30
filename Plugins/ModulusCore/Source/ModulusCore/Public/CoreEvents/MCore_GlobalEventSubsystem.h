// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreData/Types/Events/MCore_EventData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MCore_GlobalEventSubsystem.generated.h"

class UMCore_EventListenerComp;
class UMCore_GlobalEventReplicator;

/**
 * Global event subsystem for networked events
 *
 * Server-authoritative event broadcasting across all clients.
 * One instance per GameInstance (shared by all players).
 *
 * Common Uses:
 * - Game state changes (match started, round ended)
 * - World events (boss spawned, treasure chest opened)
 * - Cross-player notifications (player joined, achievement unlocked)
 * 
 * Network Setup:
 * Add UMCore_GlobalEventReplicator component to your GameState (or use AMCore_GameStateBase).
 * Without a replicator, events broadcast locally only with a warning in networked games.
 */
UCLASS(Config=ModulusGameFramework)
class MODULUSCORE_API UMCore_GlobalEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/** Validation strictness for global events (Permissive, Balanced, Strict) */
	UPROPERTY(EditAnywhere, Config, Category = "Event System Configuration")
	EMCore_ValidationStrictness ValidationStrictness = EMCore_ValidationStrictness::Balanced;

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

	/** Check if this instance has authority to broadcast global events (server-only) */
	UFUNCTION(BlueprintCallable, Category = "Event System")
	bool HasGlobalEventAuthority() const;
	
	/**
	 * Validate an event request from a client.
	 * Used by GlobalEventReplicator for RPC validation.
	 *
	 * @return true if event is valid and should be broadcast
	 */
	bool ValidateEventRequest(const FMCore_EventData& EventData) const;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	/** Registered global listener components */
	UPROPERTY()
	TArray<TWeakObjectPtr<UMCore_EventListenerComp>> GlobalListeners;
	
	/** Cached reference to the network replicator on GameState */
	TWeakObjectPtr<UMCore_GlobalEventReplicator> EventReplicator;

	/** Check if we're in a networked game (not standalone) */
	bool IsNetworkedGame() const;
};