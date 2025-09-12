// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MCore_BroadcastFunctionLibrary.generated.h"

enum class EMCore_EventScope : uint8;

/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_BroadcastFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Optimized broadcast function used by custom node.
	 * Combines validation + broadcast in single C++ call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus Events|Internal", 
		meta = (BlueprintInternalUseOnly = "true"))
	static bool OptimizedBroadcastEvent(
		UObject* WorldContext,
		const FGameplayTag& EventTag,
		EMCore_EventScope EventScope,
		const FString& Parameter
	);

	/**
	 * Quick validation function.
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus Events|Internal",
		meta = (BlueprintInternalUseOnly = "true"))
	static bool ValidateEventBroadcast(UObject* WorldContext, const FGameplayTag& EventTag);

private:
	static class UMCore_EventSubsystem* GetValidatedSubsystem(const UObject* WorldContext);
};
