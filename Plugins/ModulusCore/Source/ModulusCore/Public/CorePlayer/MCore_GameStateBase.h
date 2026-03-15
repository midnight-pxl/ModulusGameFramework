// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_GameStateBase.h
 *
 * Optional GameState base class with built-in network event support via
 * UMCore_GlobalEventReplicator. Alternative: add the component to your own GameState.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MCore_GameStateBase.generated.h"

class UMCore_GlobalEventReplicator;

/**
 * Optional GameState base class with built-in network event support.
 *
 * Blueprint Usage:
 * Set your GameMode's GameStateClass to this (or a subclass) for automatic
 * global event networking. No additional setup required.
 *
 * Alternative: Add UMCore_GlobalEventReplicator component to your own GameState.
 *
 * @see UMCore_GlobalEventReplicator
 * @see UMCore_GlobalEventSubsystem
 */
UCLASS()
class MODULUSCORE_API AMCore_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMCore_GameStateBase();

	/**
	 * Get the global event replicator component.
	 *
	 * @return The replicator component for networked event broadcasting
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus|Events")
	UMCore_GlobalEventReplicator* GetCoreGlobalEventReplicator() const { return GlobalEventReplicator; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modulus|Events")
	TObjectPtr<UMCore_GlobalEventReplicator> GlobalEventReplicator;
};
