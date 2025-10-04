// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MCore_UIFunctionLibrary.generated.h"

class UMCore_CommonUISettings;

/**
 * UI utility functions
 *
 * Helper functions for accessing UI configuration and settings.
 */
UCLASS()
class MODULUSCORE_API UMCore_UIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Get ModulusCore CommonUI settings from project settings */
	UFUNCTION(BlueprintPure, Category = "Modulus UI", CallInEditor)
	static const UMCore_CommonUISettings* GetCommonUISettings();
};
