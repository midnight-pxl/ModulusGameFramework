// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MCore_GameStateBase.generated.h"

class UMCore_GlobalEventReplicator;

/**
 * Optional GameState base class with built-in network event support.
 *
 * Usage:
 * Set your GameMode's GameStateClass to this (or a subclass) for automatic
 * global event networking. No additional setup required.
 *
 * Alternative Setup (if you have existing GameState):
 * Add UMCore_GlobalEventReplicator component to your GameState instead.
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
	
	/** Get replicator component */
	UFUNCTION(BlueprintPure, Category = "Modulus|Events")
	UMCore_GlobalEventReplicator* GetCoreGlobalEventReplicator() const { return GlobalEventReplicator; }
	
protected:
	/** Event replicator for networked event broadcasting */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modulus|Events")
	TObjectPtr<UMCore_GlobalEventReplicator> GlobalEventReplicator;
};
