// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreData/CoreStructEnums/SettingsStructsEnums/MCore_SettingsPresets.h"
#include "Engine/DataAsset.h"
#include "MCore_DA_GameSettings_Organization.generated.h"

/**
 * Simple data asset for organizing settings UI and providing defaults
 * Does NOT duplicate UE's existing knowledge, just organizes it
 */
UCLASS(BlueprintType)
class MODULUSCORE_API UMCore_DA_GameSettings_Organization : public UDataAsset
{
	GENERATED_BODY()

public:
	UMCore_DA_GameSettings_Organization();

	// UI Category Organization - Just for display purposes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Categories")
	TArray<FText> GraphicsCategories;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Categories")
	TArray<FText> AudioCategories;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Categories")
	TArray<FText> ControlsCategories;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Categories")
	TArray<FText> AccessibilityCategories;

	// Default Presets - Simple configuration only
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	EMCore_GraphicsPreset DefaultGraphicsPreset = EMCore_GraphicsPreset::Medium;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	EMCore_FrameRate DefaultFrameRate = EMCore_FrameRate::FPS_60;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	EMCore_AudioQuality DefaultAudioQuality = EMCore_AudioQuality::Medium_44kHz;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	float DefaultMouseSensitivity{1.0f};
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defaults")
	float DefaultGamepadSensitivity{1.0f};

	// Platform-specific recommendations (simple mapping)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform Defaults")
	TMap<EMCore_PlatformType, EMCore_GraphicsPreset> PlatformGraphicsDefaults;
    
	// Settings categories that should be available per platform
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform Availability")
	TMap<EMCore_PlatformType, TArray<FString>> PlatformAvailableSettings;
};
