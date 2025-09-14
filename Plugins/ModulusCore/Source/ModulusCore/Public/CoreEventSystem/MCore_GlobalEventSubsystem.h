// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MCore_EventData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MCore_GlobalEventSubsystem.generated.h"

class UMCore_EventListenerComp;

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
	 * Configuration: Should late-joining clients receive event history?
	 * RPGs/persistent worlds: true, FPS/match-based: false
	 */
	UPROPERTY(EditAnywhere, Config, Category = "Event System Configuration")
	bool bSendEventHistoryToLateJoiners{false};

	/**
	 * Configuration: How many historical events to maintain (if history enabled)
	 */
	UPROPERTY(EditAnywhere, Config, Category = "Event System Configuration", meta = (ClampMin = "10", ClampMax = "500"))
	int32 MaxEventHistorySize{50};

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
	
private:

	/**
	 * Server RPC - validate and broadcast global event
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBroadcastGlobalEvent(const FMCore_EventData& EventData);

	/**
	 * Multicast RPC - deliver global event to all clients
	 */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastGlobalEvent(const FMCore_EventData& EventData);

	/**
	 * Client RPC - send event history to late-joining client
	 */
	UFUNCTION(Client, Reliable)
	void ClientReceiveEventHistory(const TArray<FMCore_EventData>& HistoricalEvents);

	// Server authority validation
	bool ServerBroadcastGlobalEvent_Validate(const FMCore_EventData& EventData);
	void ServerBroadcastGlobalEvent_Implementation(const FMCore_EventData& EventData);
	void MulticastGlobalEvent_Implementation(const FMCore_EventData& EventData);
	void ClientReceiveEventHistory_Implementation(const TArray<FMCore_EventData>& HistoricalEvents);

	// Global listener components (server manages, clients mirror)
	UPROPERTY()
	TArray<TWeakObjectPtr<UMCore_EventListenerComp>> GlobalListeners;

	// Event history for late-joining clients (server only)
	UPROPERTY()
	TArray<FMCore_EventData> EventHistory;
	int32 EventHistoryIdx{0};

	// Internal delivery to local listeners
	void DeliverGlobalEventToLocalListeners(const FMCore_EventData& EventData);

	// Clean up stale global listener references
	void CleanupStaleGlobalListeners();
	
	/**
	 * Handle new client connections for event history
	 */
	void OnClientConnected(class APlayerController* NewPlayer);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
