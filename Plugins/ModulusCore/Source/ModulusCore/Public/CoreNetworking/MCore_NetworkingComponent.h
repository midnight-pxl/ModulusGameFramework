// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_NetworkingComponent.h
 *
 * Base networking component providing authority validation, replication helpers,
 * and Iris detection. Derive from this for network-aware actor components.
 */

#pragma once

#include "CoreMinimal.h"
#include "MCore_NetworkingInterface.h"
#include "MCore_NetworkingComponent.generated.h"

/**
 * Base networking component for ModulusCore.
 *
 * Provides authority validation, replication helpers, and Iris detection.
 * Derive from this for components that need network-aware functionality.
 *
 * Key Features:
 * - Server/client authority checks via CanExecuteServerOperation / CanExecuteClientOperation
 * - Iris replication system detection
 * - ForceNetUpdate for priority replication
 *
 * Blueprint Usage:
 * - CanExecuteServerOperation() to guard server logic
 * - CanExecuteClientOperation() to guard client logic
 * - ForceNetUpdate() to prioritize this component's replication
 */
UCLASS(Abstract, BlueprintType, ClassGroup=(ModulusCore))
class MODULUSCORE_API UMCore_NetworkingComponent : public UActorComponent, public IMCore_NetworkingInterface
{
	GENERATED_BODY()

public:
	UMCore_NetworkingComponent();

	virtual bool HasNetworkAuthority() const override
	{
		return GetOwner() ? GetOwner()->HasAuthority() : false;
	};

	/**
	 * Check if this component can execute server-authoritative operations.
	 *
	 * @return True if this component has network authority
	 */
	UFUNCTION(BlueprintPure, Category="Networking|Authority")
	bool CanExecuteServerOperation() const { return HasNetworkAuthority(); }

	/**
	 * Check if this component can execute client-side operations.
	 *
	 * @return False on dedicated server (no local player)
	 */
	UFUNCTION(BlueprintPure, Category="Networking|Authority")
	bool CanExecuteClientOperation() const
	{
		return GetOwner() ? (GetOwner()->GetNetMode() != NM_DedicatedServer) : false;
	}

	/**
	 * Check if using Unreal Engine 5's Iris replication system vs legacy.
	 *
	 * @return True if Iris replication is detected
	 */
	UFUNCTION(BlueprintPure, Category="Networking|Replication")
	bool IsUsingIrisReplication() const;

	/**
	 * Force immediate network update for this component's owning actor.
	 * Use sparingly due to performance cost.
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus|Replication")
	void ForceNetUpdate();

protected:
	// ============================================================================
	// ACTORCOMPONENT OVERRIDES
	// ============================================================================

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*
	 * NOT YET IMPLEMENTED
	 * Reserved extension point for cross-plugin authority delegation.
	 * @see EModulusAuthorityLevel
	 */
	template<typename TOperation>
	bool ExecuteWithAuthority(TOperation&& Operation, bool bRequireServerAuthority = true);

private:

	UPROPERTY(Replicated)
	FGuid ComponentNetworkID{FGuid::NewGuid()};

	UPROPERTY(Transient)
	bool bIrisDetected{false};

	void DetectNetworkingSystem();
	bool IsIrisAvailable() const;

};
