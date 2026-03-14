// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

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
 * Create child DataAssets for each theme (Minimal, Fantasy, Sci-Fi).
 * Widgets query active theme via UISubsystem->GetActiveTheme().
 *
 * Usage:
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

	/** Panel/container border style */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles")
	TSubclassOf<UCommonBorderStyle> PanelBorderStyle;

	/** Primary button style (main actions: play, confirm, submit) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles")
	TSubclassOf<UCommonButtonStyle> PrimaryButtonStyle;

	/** Secondary button style (cancel, back, navigation, settings controls) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles")
	TSubclassOf<UCommonButtonStyle> SecondaryButtonStyle;

	/**
	 * Ghost button style — no background fill, text and/or icon only with subtle hover state.
	 * Used for arrow buttons, action bar secondary actions (Reset, Back),
	 * and other de-emphasized interactive elements.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles")
	TSubclassOf<UCommonButtonStyle> GhostButtonStyle;

	/**
	 * Title/header text styles.
	 * Ordered by text size: index 0 = Small (default),
	 * index 1 = Medium, index 2 = Large.
	 * Index driven by UITextSize accessibility setting.
	 * Falls back to index 0 if requested index is not populated.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles")
	TArray<TSubclassOf<UCommonTextStyle>> TitleTextStyle;

	/** Body/content text style */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles")
	TSubclassOf<UCommonTextStyle> BodyTextStyle;

	/**
	 * Small/caption text styles.
	 * Ordered by text size: index 0 = Small (default),
	 * index 1 = Medium, index 2 = Large.
	 * Index driven by UITextSize accessibility setting.
	 * Falls back to index 0 if requested index is not populated.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles")
	TArray<TSubclassOf<UCommonTextStyle>> CaptionTextStyle;

	//~ Begin ModulusCore Style DataAssets

	/** Slider styling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style DataAssets")
	TObjectPtr<UMCore_PDA_SliderStyle> SliderStyle;

	/** Scrollbar styling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style DataAssets")
	TObjectPtr<UMCore_PDA_ScrollbarStyle> ScrollbarStyle;

	//~ End ModulusCore Style DataAssets
};
