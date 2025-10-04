// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreData/CoreStructEnums/EventStructsEnums/MCore_EventData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MCore_GlobalEventSubsystem.generated.h"

class UMCore_EventListenerComp;
class AGameModeBase;
class APlayerController;

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
 * Network: Server-authoritative
 * - Server: Broadcasts to all clients via multicast RPC
 * - Clients: Can request server to broadcast (subject to validation)
 * - Late-joiners: Do not receive events broadcast before they joined
 *
 * Blueprint Usage:
 * Use UMCore_EventFunctionLibrary::BroadcastGlobalEvent() instead of accessing this subsystem directly.
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
	 * Server: Directly multicasts to all clients
	 * Client: Sends server RPC (subject to validation based on ValidationStrictness)
	 *
	 * Use UMCore_EventFunctionLibrary::BroadcastGlobalEvent() instead of calling this directly.
	 */
	void BroadcastGlobalEvent(const FMCore_EventData& EventData);

	/** Register listener for global events. Called automatically by UMCore_EventListenerComp::BeginPlay() */
	void RegisterGlobalListener(UMCore_EventListenerComp* ListenerComponent);

	/** Unregister listener from global events. Called automatically by UMCore_EventListenerComp::EndPlay() */
	void UnregisterGlobalListener(UMCore_EventListenerComp* ListenerComponent);

	/** Check if this instance has authority to broadcast global events (server-only) */
	UFUNCTION(BlueprintCallable, Category = "Event System")
	bool HasGlobalEventAuthority() const;

	/**
	 * Server RPC - Client requests server to broadcast global event
	 *
	 * Server validates request based on ValidationStrictness before broadcasting.
	 * Do not call directly - use BroadcastGlobalEvent() instead.
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBroadcastGlobalEvent(const FMCore_EventData& EventData);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:

	/** Multicast RPC - Server delivers global event to all clients */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastGlobalEvent(const FMCore_EventData& EventData);

	// RPC implementations
	bool ServerBroadcastGlobalEvent_Validate(const FMCore_EventData& EventData);
	void ServerBroadcastGlobalEvent_Implementation(const FMCore_EventData& EventData);
	void MulticastGlobalEvent_Implementation(const FMCore_EventData& EventData);

	// Registered listener components
	UPROPERTY()
	TArray<TWeakObjectPtr<UMCore_EventListenerComp>> GlobalListeners;

	// Deliver event to listeners on this client
	void DeliverGlobalEventToLocalListeners(const FMCore_EventData& EventData);
};