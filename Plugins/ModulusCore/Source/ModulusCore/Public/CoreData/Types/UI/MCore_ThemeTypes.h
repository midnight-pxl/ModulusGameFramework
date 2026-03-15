// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_ThemeTypes.h
 *
 * Data types for the theme system. FMCore_ThemeEntry maps a display name
 * and description to a soft-referenced theme DataAsset.
 */

#pragma once

#include "CoreMinimal.h"
#include "MCore_ThemeTypes.generated.h"

class UMCore_PDA_UITheme_Base;

USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_ThemeEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DisplayName{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Description{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UMCore_PDA_UITheme_Base> ThemeAsset{ nullptr };

	bool IsValid() const { return !ThemeAsset.IsNull(); }
};
