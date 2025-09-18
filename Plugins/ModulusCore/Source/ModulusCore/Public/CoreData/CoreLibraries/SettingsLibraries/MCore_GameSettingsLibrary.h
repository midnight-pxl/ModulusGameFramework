// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreData/CoreStructEnums/SettingsStructsEnums/MCore_SettingsPresets.h"
#include "MCore_GameSettingsLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_GameSettingsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
    // UE GameUserSettings Accessor
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings Access")
    static UGameUserSettings* GetGameUserSettings();

    // Graphics Settings
    UFUNCTION(BlueprintCallable, Category = "Graphics")
    static void ApplyGraphicsPreset(EMCore_GraphicsPreset Preset);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Graphics")
	static EMCore_GraphicsPreset GetCurrentGraphicsPreset();
    
    UFUNCTION(BlueprintCallable, Category = "Graphics")
    static void SetResolution(FIntPoint Resolution, EWindowMode::Type WindowMode = EWindowMode::Fullscreen);
    
    UFUNCTION(BlueprintCallable, Category = "Graphics")
    static void SetVSyncEnabled(bool bIsEnabled);
	
	UFUNCTION(BlueprintCallable, Category = "Graphics")
	static EMCore_FrameRate GetFrameRateLimit();

	UFUNCTION(BlueprintCallable, Category = "Graphics")
	static void SetFrameRateLimit(EMCore_FrameRate FrameRate);
    
    UFUNCTION(BlueprintCallable, Category = "Graphics")
    static void SetTextureQuality(int32 Quality);
    
    UFUNCTION(BlueprintCallable, Category = "Graphics")
    static void SetShadowQuality(int32 Quality);

	UFUNCTION(BlueprintCallable, Category = "Graphics")
	static void SetAntiAliasingQuality(int32 Quality);
    
	UFUNCTION(BlueprintCallable, Category = "Graphics")
	static void SetViewDistanceQuality(int32 Quality);
    
	UFUNCTION(BlueprintCallable, Category = "Graphics")
	static void SetFoliageQuality(int32 Quality);
    
	UFUNCTION(BlueprintCallable, Category = "Graphics")
	static void SetShadingQuality(int32 Quality);

	/**
	 * Audio Settings
	 */
	UFUNCTION(BlueprintCallable, Category = "Audio")
	static void SetMasterVolume(float Volume);
    
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Audio")
	static float GetMasterVolume();
    
	UFUNCTION(BlueprintCallable, Category = "Audio")
	static void SetAudioQuality(EMCore_AudioQuality Quality);
    
	UFUNCTION(BlueprintCallable, Category = "Audio")
	static void SetAudioDevice(const FString& DeviceName);

	/**
	 * Audio Extension Placeholders -- See documentation for Sound Class integration
	 */
	UFUNCTION(BlueprintCallable, Category = "Audio")
	static void SetMusicVolume(float Volume);
    
	UFUNCTION(BlueprintCallable, Category = "Audio")
	static void SetSFXVolume(float Volume);
    
	UFUNCTION(BlueprintCallable, Category = "Audio")
	static void SetVoiceVolume(float Volume);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hardware Detection")
	static TArray<FString> GetAvailableAudioDevices();
	
	// Basic Implementations    
	UFUNCTION(BlueprintCallable, Category = "Audio")
	static void SetSubtitlesEnabled(bool bIsEnabled);
    
	UFUNCTION(BlueprintCallable, Category = "Audio")
	static void SetSubtitleSize(float Scale);
	
	/**
	 *Controls Settings
	 */	
	UFUNCTION(BlueprintCallable, Category = "Controls")
	static void SetMouseLeftRightSensitivity(float Sensitivity);

	UFUNCTION(BlueprintCallable, Category = "Controls")
	static void SetMouseUpDownSensitivity(float Sensitivity);
    
	UFUNCTION(BlueprintCallable, Category = "Controls")
	static void SetMouseInvertY(bool bInvert);
    
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controls")
	static float GetMouseLeftRightSensitivity();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controls")
	static float GetMouseUpDownSensitivity();
    
	UFUNCTION(BlueprintCallable, Category = "Controls")
	static void SetGamepadEnabled(bool bIsEnabled);

	UFUNCTION(BlueprintCallable, Category = "Controls")
	static void SetGamepadLeftRightSensitivity(float Sensitivity);
    
	UFUNCTION(BlueprintCallable, Category = "Controls")
	static void SetGamepadUpDownSensitivity(float Sensitivity);
    
	UFUNCTION(BlueprintCallable, Category = "Controls")
	static void SetGamepadVibration(bool bIsEnabled);
    
	UFUNCTION(BlueprintCallable, Category = "Controls")
	static void SetGamepadInvertY(bool bInvert);
    
	UFUNCTION(BlueprintCallable, Category = "Controls")
	static void SetGamepadDeadZone(float DeadZone);
    
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controls")
	static EMCore_InputDeviceType GetCurrentInputDevice();

	/**
	 * Enhanced Input Key Remappings
	 */
	UFUNCTION(BlueprintCallable, Category = "Controls", meta = (ToolTip = "Requires Enhanced Input setup with Player Mappable Input Config"))
	static bool RemapInputAction(const FString& ActionName, FKey NewKey);
    
	UFUNCTION(BlueprintCallable, Category = "Controls", meta = (ToolTip = "Requires Enhanced Input setup with Player Mappable Input Config"))
	static void ResetInputMappingsToDefault();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controls")
	static TArray<FString> GetRemappableActionNames();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controls") 
	static FKey GetCurrentKeyForAction(const FString& ActionName);
	
    /**
     * Accessibility Settings
     */
	UFUNCTION(BlueprintCallable, Category = "Accessibility")
	static void SetColorBlindMode(EMCore_ColorBlindType ColorBlindType);
    
	UFUNCTION(BlueprintCallable, Category = "Accessibility")
	static void SetUITextScale(float Scale);
    
	UFUNCTION(BlueprintCallable, Category = "Accessibility")
	static void SetHighContrastMode(bool bIsEnabled);
    
	UFUNCTION(BlueprintCallable, Category = "Accessibility")
	static void SetPhotosensitivityReduction(bool bIsEnabled);

    /**
     * Hardware Detection - For system access
     */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hardware Detection")
	static TArray<FIntPoint> GetSupportedResolutions();
    
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hardware Detection")
	static bool SupportsRayTracing();
    
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hardware Detection")
	static bool SupportsHDR();
    
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hardware Detection")
	static TArray<FString> GetAvailableUpscalingMethods();

    /**
     * Platform Detection - For system queries
     */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Platform Detection")
	static EMCore_PlatformType GetCurrentPlatform();
    
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Platform Detection")
	static bool IsConsole();
    
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Platform Detection")
	static bool IsMobile();
    
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Platform Detection")
	static bool IsSteamDeck();

	/**
	 * Settings Management - Simple UE system integration
	 */
	UFUNCTION(BlueprintCallable, Category = "Settings Management")
	static void ApplyAndSaveAllSettings();
    
	UFUNCTION(BlueprintCallable, Category = "Settings Management")
	static void ResetAllSettingsToDefaults();

private:
	// Internal helper functions
	static int32 ConvertPresetToScalabilityLevel(EMCore_GraphicsPreset Preset);
	static EMCore_GraphicsPreset ConvertScalabilityLevelToPreset(int32 Level);
	static float ConvertFrameRateEnumToFloat(EMCore_FrameRate FrameRate);
	static EMCore_FrameRate ConvertFloatToFrameRateEnum(float FrameRateFloat);

	// Settings storage helpers
	static float GetStoredMasterVolume();
	static void SetStoredMasterVolume(float Volume);
	static float GetStoredMouseSensitivity(bool bYAxis = false);
	static void SetStoredMouseSensitivity(float Sensitivity, bool bYAxis = false);
};
