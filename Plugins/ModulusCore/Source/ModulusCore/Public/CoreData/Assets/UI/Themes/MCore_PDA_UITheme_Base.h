// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_PDA_UITheme_Base.h
 *
 * Master UI Theme DataAsset referencing CommonUI styles and modular style DataAssets.
 * Create child DataAssets for each theme (Minimal, Fantasy, Sci-Fi).
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MCore_PDA_UITheme_Base.generated.h"

class UCommonButtonStyle;
class UCommonTextStyle;
class UCommonBorderStyle;
class UMCore_PDA_SliderStyle;
class UMCore_PDA_ScrollbarStyle;

/**
 * Master UI Theme DataAsset - references CommonUI styles and modular style DataAssets.
 *
 * Widgets query the active theme via UISubsystem->GetActiveTheme().
 *
 * Blueprint Usage:
 *   1. Create CommonUI styles (BP_ButtonStyle_Primary, BP_TextStyle_Body, etc.)
 *   2. Create style DataAssets for sliders/scrollbars if needed
 *   3. Create theme DataAsset referencing all pieces
 *   4. Set as active theme in Project Settings or via UISubsystem
 */
UCLASS(Abstract, Blueprintable)
class MODULUSCORE_API UMCore_PDA_UITheme_Base : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// ============================================================================
	// COMMONUI STYLES
	// ============================================================================

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles")
	TSubclassOf<UCommonBorderStyle> PanelBorderStyle;

	/* Main actions: play, confirm, submit */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles")
	TSubclassOf<UCommonButtonStyle> PrimaryButtonStyle;

	/* Cancel, back, navigation, settings controls */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles")
	TSubclassOf<UCommonButtonStyle> SecondaryButtonStyle;

	/* No background fill, text/icon only with subtle hover. Used for arrow buttons and de-emphasized actions. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles")
	TSubclassOf<UCommonButtonStyle> GhostButtonStyle;

	/*
	 * Title/header text styles ordered by size: [0]=Small (default), [1]=Medium, [2]=Large.
	 * Index driven by UITextSize accessibility setting. Falls back to index 0.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles")
	TArray<TSubclassOf<UCommonTextStyle>> TitleTextStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles")
	TSubclassOf<UCommonTextStyle> BodyTextStyle;

	/*
	 * Small/caption text styles ordered by size: [0]=Small (default), [1]=Medium, [2]=Large.
	 * Index driven by UITextSize accessibility setting. Falls back to index 0.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles")
	TArray<TSubclassOf<UCommonTextStyle>> CaptionTextStyle;

	// ============================================================================
	// MODULUSCORE STYLE DATEASSETS
	// ============================================================================

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style DataAssets")
	TObjectPtr<UMCore_PDA_SliderStyle> SliderStyle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style DataAssets")
	TObjectPtr<UMCore_PDA_ScrollbarStyle> ScrollbarStyle;
};
