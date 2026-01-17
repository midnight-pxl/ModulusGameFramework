// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreData/Types/Settings/MCore_SettingsPresets.h"
#include "Engine/DataAsset.h"
#include "MCore_DA_GameSettings_Organization.generated.h"

/**
 * Game settings organization data asset
 *
 * Stores default settings values and UI category organization for your game.
 * Create one instance and reference it from your settings menu.
 *
 * Example Setup:
 * - DefaultGraphicsPreset: Medium (or High for powerful target platforms)
 * - DefaultFrameRate: FPS_60 (console) or Unlimited (PC)
 * - PlatformGraphicsDefaults: PC=Ultra, Console=High, Mobile=Low
 *
 * Blueprint Usage:
 * 1. Create data asset instance in Content Browser
 * 2. Configure defaults for your target platforms
 * 3. Reference from settings menu widget
 * 4. Apply defaults on first launch or reset
 */
UCLASS(BlueprintType)
class MODULUSCORE_API UMCore_DA_GameSettings_Organization : public UDataAsset
{
	GENERATED_BODY()

public:
	UMCore_DA_GameSettings_Organization();

	/** Category labels for graphics settings UI (e.g., "Display", "Quality", "Advanced") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Categories")
	TArray<FText> GraphicsCategories;

	/** Category labels for audio settings UI (e.g., "Volume", "Audio Quality") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Categories")
	TArray<FText> AudioCategories;

	/** Category labels for controls settings UI (e.g., "Mouse", "Keyboard", "Gamepad") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Categories")
	TArray<FText> ControlsCategories;

	/** Category labels for accessibility settings UI (e.g., "Visual", "Audio", "Input") */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Categories")
	TArray<FText> AccessibilityCategories;

	/** Default graphics quality preset for first launch */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	EMCore_GraphicsPreset DefaultGraphicsPreset = EMCore_GraphicsPreset::Medium;

	/** Default frame rate limit for first launch */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	EMCore_FrameRate DefaultFrameRate = EMCore_FrameRate::FPS_60;

	/** Default audio quality for first launch */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	EMCore_AudioQuality DefaultAudioQuality = EMCore_AudioQuality::Medium_44kHz;

	/** Default mouse sensitivity (0.1 to 3.0) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	float DefaultMouseSensitivity{1.0f};

	/** Default gamepad sensitivity (0.1 to 3.0) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	float DefaultGamepadSensitivity{1.0f};

	/** Graphics preset recommendations per platform (PC, Console, Mobile, SteamDeck) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform Defaults")
	TMap<EMCore_PlatformType, EMCore_GraphicsPreset> PlatformGraphicsDefaults;
};
