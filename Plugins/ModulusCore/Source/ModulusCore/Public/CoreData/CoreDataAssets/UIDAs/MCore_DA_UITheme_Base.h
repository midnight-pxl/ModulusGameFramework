// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MCore_DA_UITheme_Base.generated.h"

/**
 * UI theme data asset base class
 *
 * Base class for UI theming data assets. Currently a placeholder for future theming system.
 * Extend this class to create custom UI themes for your game.
 *
 * Future Features:
 * - Color palettes (primary, secondary, accent, error colors)
 * - Font styles (headings, body, buttons)
 * - Widget styles (button, slider, dropdown appearances)
 * - Spacing/padding standards
 */
UCLASS()
class MODULUSCORE_API UMCore_DA_UITheme_Base : public UDataAsset
{
	GENERATED_BODY()
};
