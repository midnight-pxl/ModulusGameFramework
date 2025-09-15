// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MCore_EventData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MCore_GlobalEventSubsystem.generated.h"

class UMCore_EventListenerComp;
class AGameModeBase;
class APlayerController;

/**
 * Server-authoritative global event system
 * Handles events that need to be synchronized across all players
 */
UCLASS(Config=ModulusGameFramework)
class MODULUSCORE_API UMCore_GlobalEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/**
	 * Configuration: Authority validation strictness
	 */
	UPROPERTY(EditAnywhere, Config, Category = "Event System Configuration")
	EMCore_ValidationStrictness ValidationStrictness = EMCore_ValidationStrictness::Balanced;
	
	/**
	 * Server-side: Broadcast global event to all clients
	 */
	void BroadcastGlobalEvent(const FMCore_EventData& EventData);

	/**
	 * Register a listener component for global events (server manages this)
	 */
	void RegisterGlobalListener(UMCore_EventListenerComp* ListenerComponent);

	/**
	 * Unregister a listener component from global events
	 */
	void UnregisterGlobalListener(UMCore_EventListenerComp* ListenerComponent);

	// Server authority checking
	UFUNCTION(BlueprintCallable, Category = "Event System")
	bool HasGlobalEventAuthority() const;

	/**
	 * Server RPC - validate and broadcast global event
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBroadcastGlobalEvent(const FMCore_EventData& EventData);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:

	/**
	 * Multicast RPC - deliver global event to all clients
	 */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastGlobalEvent(const FMCore_EventData& EventData);

	// Server authority validation
	bool ServerBroadcastGlobalEvent_Validate(const FMCore_EventData& EventData);
	void ServerBroadcastGlobalEvent_Implementation(const FMCore_EventData& EventData);
	void MulticastGlobalEvent_Implementation(const FMCore_EventData& EventData);

	// Global listener components (server manages, clients mirror)
	UPROPERTY()
	TArray<TWeakObjectPtr<UMCore_EventListenerComp>> GlobalListeners;

	// Internal delivery to local listeners
	void DeliverGlobalEventToLocalListeners(const FMCore_EventData& EventData);
};