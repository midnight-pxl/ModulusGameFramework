// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MCore_PDA_UITheme_Base.generated.h"

class UCommonButtonStyle;
class UCommonTextStyle;
class UCommonBorderStyle;
class UMCore_PDA_MenuStyle;
class UMCore_PDA_SliderStyle;
class UMCore_PDA_ScrollbarStyle;
class UMCore_PDA_SoundStyle;

/**
 * Master UI Theme DataAsset - references modular style DataAssets.
 * 
 * Create child DataAssets for each theme (Minimal, Fantasy, Sci-Fi).
 * Widgets query active theme via UISubsystem->GetActiveTheme().
 * 
 * Architecture:
 *   - CommonUI style references for buttons/text (native CommonUI assets)
 *   - Modular DataAsset references for sliders, scrollbars, sounds, animations
 *   - Mix and match styles across themes
 * 
 * Usage:
 *   1. Create style DataAssets (DA_SliderStyle_Minimal, etc.)
 *   2. Create CommonUI styles (BP_ButtonStyle_Primary, etc.)
 *   3. Create theme DataAsset referencing all pieces
 *   4. Set as active theme in Project Settings or via UISubsystem
 */
UCLASS(Abstract, Blueprintable)
class MODULUSCORE_API UMCore_PDA_UITheme_Base : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	/** Panel/container border style */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles|Borders")
	TSubclassOf<UCommonBorderStyle> PanelBorderStyle;

	/** Input field border style */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles|Borders")
	TSubclassOf<UCommonBorderStyle> InputBorderStyle;
	
	/** Primary button style (main menu buttons, confirm actions) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles|Buttons")
	TSubclassOf<UCommonButtonStyle> PrimaryButtonStyle;

	/** Secondary button style (cancel, back, less prominent) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles|Buttons")
	TSubclassOf<UCommonButtonStyle> SecondaryButtonStyle;

	/** Navigation button style (tabs, arrows, pagination) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles|Buttons")
	TSubclassOf<UCommonButtonStyle> NavigationButtonStyle;

	/** Settings control button style (dropdowns, toggles) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles|Buttons")
	TSubclassOf<UCommonButtonStyle> SettingsButtonStyle;

	/** Destructive action button style (quit, delete) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles|Buttons")
	TSubclassOf<UCommonButtonStyle> DestructiveButtonStyle;

	/** Title/header text style */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles|Text")
	TSubclassOf<UCommonTextStyle> TitleTextStyle;

	/** Body/content text style */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles|Text")
	TSubclassOf<UCommonTextStyle> BodyTextStyle;

	/** Button label text style */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles|Text")
	TSubclassOf<UCommonTextStyle> ButtonTextStyle;

	/** Small/caption text style */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CommonUI Styles|Text")
	TSubclassOf<UCommonTextStyle> CaptionTextStyle;

	// =========================================================================
	// ModulusCore Style DataAssets
	// =========================================================================
	
	/** Menu styling (colors, backgrounds, layout, branding) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style DataAssets")
	TObjectPtr<UMCore_PDA_MenuStyle> MenuStyle;

	/** Slider styling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style DataAssets")
	TObjectPtr<UMCore_PDA_SliderStyle> SliderStyle;

	/** Scrollbar styling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style DataAssets")
	TObjectPtr<UMCore_PDA_ScrollbarStyle> ScrollbarStyle;

	/** UI sound styling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Style DataAssets")
	TObjectPtr<UMCore_PDA_SoundStyle> SoundStyle;
	
};







