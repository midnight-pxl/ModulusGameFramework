// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MCore_NetworkingInterface.h"
#include "MCore_NetworkingComponent.generated.h"

UCLASS(Abstract, BlueprintType, ClassGroup=(ModulusCore))
class MODULUSCORE_API UMCore_NetworkingComponent : public UActorComponent, public IMCore_NetworkingInterface
{
	GENERATED_BODY()

public:
	UMCore_NetworkingComponent();

	// Authority Validations	
	virtual bool HasNetworkAuthority() const override
	{
		return GetOwner() ? GetOwner()->HasAuthority() : false;
	};

	UFUNCTION(BlueprintPure, Category="Networking|Authority")
	bool CanExecuteServerOperation() const { return HasNetworkAuthority(); }

	UFUNCTION(BlueprintPure, Category="Networking|Authority")
	bool CanExecuteClientOperation() const
	{
		return GetOwner() ? (GetOwner()->GetNetMode() != NM_DedicatedServer) : false;
	}

	UFUNCTION(BlueprintPure, Category="Networking|Replication")
	bool IsUsingIrisReplication() const;
	
	// Performance Utility
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