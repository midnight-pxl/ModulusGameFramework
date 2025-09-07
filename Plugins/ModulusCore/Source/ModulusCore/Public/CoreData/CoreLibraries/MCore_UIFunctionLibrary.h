// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MCore_UIFunctionLibrary.generated.h"

class FGameplayTag;

/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_UIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Modulus UI", CallInEditor = true)
	static const UMCore_CommonUISettings* GetCommonUISettings();

	// Helper functions for tag validation
	UFUNCTION(BlueprintPure, Category = "Modulus UI", CallInEditor = true)
	static FGameplayTag GetValidatedGameplayTag(const FString& TagString, const FString& FallbackTag = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = "Modulus UI", CallInEditor = true)
	static bool ValidateUILayerTags(TArray<FString>& OutInvalidTags);
};
