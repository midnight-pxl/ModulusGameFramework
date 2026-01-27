// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MCore_GlobalEventReplicator.generated.h"

struct FMCore_EventData;
class UMCore_GlobalEventSubsystem;

/**
 * Replicated component that handles global event network transport.
 *
 * Attach to GameState to enable cross-network event broadcasting.
 * The GlobalEventSubsystem discovers this component automatically
 * and routes RPC calls through it.
 *
 * Setup Options:
 * 1. Use AMCore_GameStateBase as your GameMode's GameStateClass (zero setup)
 * 2. Add this component to your GameState Blueprint
 * 3. CreateDefaultSubobject in your C++ GameState constructor
 *
 * Note: This component is pure network transport. All business logic
 * (listener management, validation rules) lives in GlobalEventSubsystem.
 */
UCLASS(ClassGroup=(ModulusCore), meta=(BlueprintSpawnableComponent, DisplayName="Global Event Replicator"))
class MODULUSCORE_API UMCore_GlobalEventReplicator : public UActorComponent
{
	GENERATED_BODY()

public:
	UMCore_GlobalEventReplicator();

	/**
	 * Request a global event broadcast.
	 * Routes to server RPC or direct multicast based on authority.
	 * Called by GlobalEventSubsystem - do not call directly.
	 *
	 * @param EventData The event to broadcast
	 */
	void RequestBroadcast(const FMCore_EventData& EventData);
	
protected:
	//~ Begin UActorComponent Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End UActorComponent Interface
	
	/**
	 * Client -> Server: Request event broadcast
	 * Server validates then multicasts to all clients
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestBroadcast(const FMCore_EventData& EventData);

	/**
	 * Server -> All Clients: Deliver event
	 * Executes on server and all connected clients
	 */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastToClients(const FMCore_EventData& EventData);

private:
	/** Find and cache the GlobalEventSubsystem */
	UMCore_GlobalEventSubsystem* GetEventSubsystem() const;
	
	/** Cached reference to avoid repeated lookups */
	mutable TWeakObjectPtr<UMCore_GlobalEventSubsystem> CachedSubsystem;
};
