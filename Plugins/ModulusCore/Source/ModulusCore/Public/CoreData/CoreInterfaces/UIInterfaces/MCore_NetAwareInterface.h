// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MCore_NetAwareInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UMCore_NetAwareInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MODULUSCORE_API IMCore_NetAwareInterface
{
	GENERATED_BODY()

public:
	// Authority Validation
	UFUNCTION(BlueprintCallable, Category = "Modulus|Network")
	bool HasNetworkAuthority() const;

	UFUNCTION(BlueprintCallable, Category = "Modulus|Network")
	bool CanExecuteServerOperation() const;

	UFUNCTION(BlueprintCallable, Category = "Modulus|Network")
	bool CanExecuteClientOperation() const;

	// Network Mode Queries
	UFUNCTION(BlueprintPure, Category = "Modulus|Network")
	bool IsServer() const;

	UFUNCTION(BlueprintPure, Category = "Modulus|Network")
	bool IsClient() const;
    
	UFUNCTION(BlueprintPure, Category = "Modulus|Network")
	bool IsDedicatedServer() const;
    
	UFUNCTION(BlueprintPure, Category = "Modulus|Network")
	bool IsListenServer() const;

	// Context Access
	UFUNCTION(BlueprintCallable, Category = "Modulus|Network")
	APlayerController* GetOwningPlayerController() const;
    
	UFUNCTION(BlueprintCallable, Category = "Modulus|Network")
	APawn* GetOwningPawn() const;
    
	template<typename T>
	T* GetOwningPawn() const
	{
		return Cast<T>(GetOwningPawn());
	}

};
