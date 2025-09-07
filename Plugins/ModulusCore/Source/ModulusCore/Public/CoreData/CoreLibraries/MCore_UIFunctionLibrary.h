// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MCore_UIFunctionLibrary.generated.h"

class UMCore_CommonUISettings;

/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_UIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Modulus UI", CallInEditor)
	static const UMCore_CommonUISettings* GetCommonUISettings();
};
