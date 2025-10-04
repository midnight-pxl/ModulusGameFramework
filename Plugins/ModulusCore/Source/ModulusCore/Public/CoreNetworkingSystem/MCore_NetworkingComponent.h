// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MCore_NetworkingInterface.h"
#include "MCore_NetworkingComponent.generated.h"

/**
 * Base networking component for ModulusCore
 *
 * Provides authority validation, replication helpers, and Iris detection.
 * Derive from this for components that need network-aware functionality.
 *
 * Common Uses:
 * - Event listener components (UMCore_EventListenerComp)
 * - Replicated gameplay components
 * - Components that need server/client authority checks
 *
 * Blueprint Usage:
 * - CanExecuteServerOperation() to check if component can run server logic
 * - CanExecuteClientOperation() to check if component can run client logic
 * - ForceNetUpdate() to prioritize this component's replication
 */
UCLASS(Abstract, BlueprintType, ClassGroup=(ModulusCore))
class MODULUSCORE_API UMCore_NetworkingComponent : public UActorComponent, public IMCore_NetworkingInterface
{
	GENERATED_BODY()

public:
	UMCore_NetworkingComponent();

	/** Check if this component has network authority (server in client-server, or standalone) */
	virtual bool HasNetworkAuthority() const override
	{
		return GetOwner() ? GetOwner()->HasAuthority() : false;
	};

	/** Check if this component can execute server-authoritative operations */
	UFUNCTION(BlueprintPure, Category="Networking|Authority")
	bool CanExecuteServerOperation() const { return HasNetworkAuthority(); }

	/** Check if this component can execute client-side operations (false on dedicated server) */
	UFUNCTION(BlueprintPure, Category="Networking|Authority")
	bool CanExecuteClientOperation() const
	{
		return GetOwner() ? (GetOwner()->GetNetMode() != NM_DedicatedServer) : false;
	}

	/** Check if using Unreal Engine 5's Iris replication system (vs legacy replication) */
	UFUNCTION(BlueprintPure, Category="Networking|Replication")
	bool IsUsingIrisReplication() const;

	/** Force immediate network update for this component (use sparingly - performance cost) */
	UFUNCTION(BlueprintCallable, Category = "Modulus|Replication")
	void ForceNetUpdate();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** TODO - Uncomment if using Iris Replication System (Remove line)
	virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, 
											   UE::Net::EFragmentRegistrationFlags RegistrationFlags) override;
	* Remove line */
	
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