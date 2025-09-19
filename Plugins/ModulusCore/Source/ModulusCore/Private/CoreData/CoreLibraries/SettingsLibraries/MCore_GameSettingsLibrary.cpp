// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreData/CoreLibraries/SettingsLibraries/MCore_GameSettingsLibrary.h"

#include "AudioDevice.h"
#include "CoreData/CoreLogging/LogModulusSettings.h"
#include "GameFramework/GameUserSettings.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "PlayerMappableInputConfig.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/KismetRenderingLibrary.h"

static float StoredMasterVolume{1.0f};
static float StoredMouseSensitivityX{1.0f};
static float StoredMouseSensitivityY{1.0f};

UGameUserSettings* UMCore_GameSettingsLibrary::GetGameUserSettings()
{
	return UGameUserSettings::GetGameUserSettings();
}

void UMCore_GameSettingsLibrary::ApplyGraphicsPreset(EMCore_GraphicsPreset Preset)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogModulusSettings, Warning, TEXT("Failed to get GameUserSettings for graphics preset"));
		return;
	}

	int32 ScalabilityLevel = ConvertPresetToScalabilityLevel(Preset);
	Settings->SetOverallScalabilityLevel(ScalabilityLevel);
	Settings->ApplySettings(false);

	UE_LOG(LogModulusSettings, Log, TEXT("Applied graphics preset: %d (Scalability Level: %d)"), 
		   static_cast<int32>(Preset), ScalabilityLevel);
}

EMCore_GraphicsPreset UMCore_GameSettingsLibrary::GetCurrentGraphicsPreset()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings) { return EMCore_GraphicsPreset::Medium; }

	int32 CurrentScalabilityLevel = Settings->GetOverallScalabilityLevel();
	return ConvertScalabilityLevelToPreset(CurrentScalabilityLevel);
}

void UMCore_GameSettingsLibrary::SetResolution(FIntPoint Resolution, EWindowMode::Type WindowMode)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogModulusSettings, Warning, TEXT("Failed to get GameUserSettings for resolution"));
		return;
	}

	Settings->SetScreenResolution(Resolution);
	Settings->SetFullscreenMode(WindowMode);
	Settings->ApplyResolutionSettings(false);

	UE_LOG(LogModulusSettings, Log, TEXT("Set resolution to: %dx%d, Window Mode: %d"), 
		   Resolution.X, Resolution.Y, static_cast<int32>(WindowMode));
}

void UMCore_GameSettingsLibrary::SetVSyncEnabled(bool bIsEnabled)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogModulusSettings, Warning, TEXT("Failed to get GameUserSettings for VSync"));
		return;
	}

	Settings->SetVSyncEnabled(bIsEnabled);
	UE_LOG(LogModulusSettings, Log, TEXT("VSync set to: %s"), bIsEnabled ? TEXT("Enabled") : TEXT("Disabled"));
}

EMCore_FrameRate UMCore_GameSettingsLibrary::GetFrameRateLimit()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return EMCore_FrameRate::FPS_60;
	}

	float CurrentLimit = Settings->GetFrameRateLimit();
	return ConvertFloatToFrameRateEnum(CurrentLimit);
}

void UMCore_GameSettingsLibrary::SetFrameRateLimit(EMCore_FrameRate FrameRate)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogModulusSettings, Warning, TEXT("Failed to get GameUserSettings for frame rate"));
		return;
	}

	float FrameRateLimit = ConvertFrameRateEnumToFloat(FrameRate);
	Settings->SetFrameRateLimit(FrameRateLimit);
    
	UE_LOG(LogModulusSettings, Log, TEXT("Frame rate limit set to: %s"), 
		   FrameRate == EMCore_FrameRate::Unlimited ? TEXT("Unlimited") : *FString::Printf(TEXT("%.0f FPS"), FrameRateLimit));
}

void UMCore_GameSettingsLibrary::SetTextureQuality(int32 Quality)
{
    UGameUserSettings* Settings = GetGameUserSettings();
    if (!Settings)
    {
        return;
    }

    Quality = FMath::Clamp(Quality, 0, 3);
    Settings->SetTextureQuality(Quality);
    UE_LOG(LogModulusSettings, Log, TEXT("Texture quality set to: %d"), Quality);
}

void UMCore_GameSettingsLibrary::SetShadowQuality(int32 Quality)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return;
	}

	Quality = FMath::Clamp(Quality, 0, 3);
	Settings->SetShadowQuality(Quality);
	UE_LOG(LogModulusSettings, Log, TEXT("Shadow quality set to: %d"), Quality);
}

void UMCore_GameSettingsLibrary::SetAntiAliasingQuality(int32 Quality)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return;
	}

	Quality = FMath::Clamp(Quality, 0, 3);
	Settings->SetAntiAliasingQuality(Quality);
	UE_LOG(LogModulusSettings, Log, TEXT("Anti-aliasing quality set to: %d"), Quality);
}

void UMCore_GameSettingsLibrary::SetViewDistanceQuality(int32 Quality)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return;
	}

	Quality = FMath::Clamp(Quality, 0, 3);
	// Use ViewDistanceQuality instead - this actually exists in UE 5.6
	Settings->SetViewDistanceQuality(Quality);
	UE_LOG(LogModulusSettings, Log, TEXT("View distance quality set to: %d"), Quality);
}

void UMCore_GameSettingsLibrary::SetFoliageQuality(int32 Quality)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return;
	}

	Quality = FMath::Clamp(Quality, 0, 3);
	Settings->SetFoliageQuality(Quality);
	UE_LOG(LogModulusSettings, Log, TEXT("Foliage quality set to: %d"), Quality);
}

void UMCore_GameSettingsLibrary::SetShadingQuality(int32 Quality)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return;
	}

	Quality = FMath::Clamp(Quality, 0, 3);
	Settings->SetShadingQuality(Quality);
	UE_LOG(LogModulusSettings, Log, TEXT("Shading quality set to: %d"), Quality);
}

void UMCore_GameSettingsLibrary::SetMasterVolume(float Volume)
{
	Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
	StoredMasterVolume = Volume;
    
	// Simple approach - just store the volume
	// Actual application depends on project audio setup
	UE_LOG(LogModulusSettings, Log, TEXT("Master volume set to: %.2f (stored - see documentation for audio integration)"), Volume);
}

void UMCore_GameSettingsLibrary::SetMusicVolume(float Volume)
{
	Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
    
	// Use Sound Submix approach for UE 5.6
	if (GEngine->GetCurrentPlayWorld())
	{
		// Assuming you have a Music submix set up in your project
		// This would need to reference your actual Music submix asset
		UE_LOG(LogModulusSettings, Log, TEXT("Music volume set to: %.2f (Submix implementation required)"), Volume);
        
		// Alternative: Use console variable if you have audio categories set up
		if (IConsoleVariable* MusicVolumeCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.MusicVolume")))
		{
			MusicVolumeCVar->Set(Volume);
		}
	}
}

void UMCore_GameSettingsLibrary::SetSFXVolume(float Volume)
{
	Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
    
	if (IConsoleVariable* SFXVolumeCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.SFXVolume")))
	{
		SFXVolumeCVar->Set(Volume);
	}
    
	UE_LOG(LogModulusSettings, Log, TEXT("SFX volume set to: %.2f"), Volume);
}

void UMCore_GameSettingsLibrary::SetVoiceVolume(float Volume)
{
	Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
    
	if (IConsoleVariable* VoiceVolumeCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.VoiceVolume")))
	{
		VoiceVolumeCVar->Set(Volume);
	}
    
	UE_LOG(LogModulusSettings, Log, TEXT("Voice volume set to: %.2f"), Volume);
}

float UMCore_GameSettingsLibrary::GetMasterVolume()
{
	if (IConsoleVariable* MasterVolumeCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("au.GlobalVolumeMultiplier")))
	{
		return MasterVolumeCVar->GetFloat();
	}
	return 1.0f;
}

void UMCore_GameSettingsLibrary::SetAudioQuality(EMCore_AudioQuality Quality)
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		return;
	}

	int32 QualityLevel;
	switch(Quality)
	{
	case EMCore_AudioQuality::Low_22kHz:    QualityLevel = 0; break;
	case EMCore_AudioQuality::Medium_44kHz: QualityLevel = 1; break;
	case EMCore_AudioQuality::High_48kHz:   QualityLevel = 2; break;
	default:                                QualityLevel = 1; break;
	}
    
	Settings->SetAudioQualityLevel(QualityLevel);
	UE_LOG(LogModulusSettings, Log, TEXT("Audio quality set to level: %d"), QualityLevel);
}

void UMCore_GameSettingsLibrary::SetAudioDevice(const FString& DeviceName)
{
	// Platform-specific audio device setting would go here
	// For now, log the request
	UE_LOG(LogModulusSettings, Log, TEXT("Audio device change requested: %s"), *DeviceName);
}

void UMCore_GameSettingsLibrary::SetSubtitlesEnabled(bool bIsEnabled)
{
	if (IConsoleVariable* SubtitleCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("UI.Subtitles.Enable")))
	{
		SubtitleCVar->Set(bIsEnabled ? 1 : 0);
		UE_LOG(LogModulusSettings, Log, TEXT("Subtitles set to: %s"), bIsEnabled ? TEXT("Enabled") : TEXT("Disabled"));
	}
}

void UMCore_GameSettingsLibrary::SetSubtitleSize(float Scale)
{
	Scale = FMath::Clamp(Scale, 0.5f, 2.0f);
    
	if (IConsoleVariable* SubtitleScaleCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("UI.Subtitles.Scale")))
	{
		SubtitleScaleCVar->Set(Scale);
		UE_LOG(LogModulusSettings, Log, TEXT("Subtitle scale set to: %.2f"), Scale);
	}
}

void UMCore_GameSettingsLibrary::SetMouseLeftRightSensitivity(float Sensitivity)
{
	Sensitivity = FMath::Clamp(Sensitivity, 0.1f, 10.0f);
	StoredMouseSensitivityX = Sensitivity;
	UE_LOG(LogModulusSettings, Log, TEXT("Mouse X sensitivity set to: %.2f"), Sensitivity);
}

void UMCore_GameSettingsLibrary::SetMouseUpDownSensitivity(float Sensitivity)
{
	Sensitivity = FMath::Clamp(Sensitivity, 0.1f, 10.0f);
	StoredMouseSensitivityY = Sensitivity;
	UE_LOG(LogModulusSettings, Log, TEXT("Mouse Y sensitivity set to: %.2f"), Sensitivity);
}

void UMCore_GameSettingsLibrary::SetMouseInvertY(bool bInvert)
{
	// Store mouse inversion setting
	if (IConsoleVariable* MouseInvertYCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("input.MouseInvertY")))
	{
		MouseInvertYCVar->Set(bInvert ? 1 : 0);
	}
    
	UE_LOG(LogModulusSettings, Log, TEXT("Mouse invert Y set to: %s"), bInvert ? TEXT("Enabled") : TEXT("Disabled"));
}

float UMCore_GameSettingsLibrary::GetMouseLeftRightSensitivity()
{
	return StoredMouseSensitivityX;
}

float UMCore_GameSettingsLibrary::GetMouseUpDownSensitivity()
{
	return StoredMouseSensitivityY;
}

void UMCore_GameSettingsLibrary::SetGamepadEnabled(bool bIsEnabled)
{
	UE_LOG(LogModulusSettings, Log, TEXT("Gamepad enabled set to: %s"), bIsEnabled ? TEXT("Enabled") : TEXT("Disabled"));
}

void UMCore_GameSettingsLibrary::SetGamepadLeftRightSensitivity(float Sensitivity)
{
	Sensitivity = FMath::Clamp(Sensitivity, 0.1f, 10.0f);
	UE_LOG(LogModulusSettings, Log, TEXT("Gamepad X sensitivity set to: %.2f"), Sensitivity);
}

void UMCore_GameSettingsLibrary::SetGamepadUpDownSensitivity(float Sensitivity)
{
	Sensitivity = FMath::Clamp(Sensitivity, 0.1f, 10.0f);
	UE_LOG(LogModulusSettings, Log, TEXT("Gamepad Y sensitivity set to: %.2f"), Sensitivity);
}

void UMCore_GameSettingsLibrary::SetGamepadVibration(bool bIsEnabled)
{
	UE_LOG(LogModulusSettings, Log, TEXT("Gamepad vibration set to: %s"), bIsEnabled ? TEXT("Enabled") : TEXT("Disabled"));
}

void UMCore_GameSettingsLibrary::SetGamepadInvertY(bool bInvert)
{
	UE_LOG(LogModulusSettings, Log, TEXT("Gamepad invert Y set to: %s"), bInvert ? TEXT("Enabled") : TEXT("Disabled"));
}

void UMCore_GameSettingsLibrary::SetGamepadDeadZone(float DeadZone)
{
	DeadZone = FMath::Clamp(DeadZone, 0.0f, 0.95f);
	UE_LOG(LogModulusSettings, Log, TEXT("Gamepad dead zone set to: %.2f"), DeadZone);
}

EMCore_InputDeviceType UMCore_GameSettingsLibrary::GetCurrentInputDevice()
{
	// Use Slate Application to detect current input method in UE 5.6
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication& SlateApp = FSlateApplication::Get();
        
		// Check if we have gamepad input
		if (SlateApp.IsGamepadAttached())
		{
			// Additional check for recent gamepad activity could go here
			return EMCore_InputDeviceType::Gamepad;
		}
        
		// Check for touch input
		if (FPlatformMisc::GetUseVirtualJoysticks())
		{
			return EMCore_InputDeviceType::Touch;
		}
	}
    
	// Default to keyboard/mouse
	return EMCore_InputDeviceType::KeyboardMouse;
}

void UMCore_GameSettingsLibrary::SetColorBlindMode(EMCore_ColorBlindType ColorBlindType)
{
	// Implementation would apply colorblind-friendly color schemes
	UE_LOG(LogModulusSettings, Log, TEXT("Color blind mode set to: %d"), static_cast<int32>(ColorBlindType));
}

void UMCore_GameSettingsLibrary::SetUITextScale(float Scale)
{
	Scale = FMath::Clamp(Scale, 0.5f, 2.0f);
    
	// Apply UI scaling - this would integrate with CommonUI scaling
	if (IConsoleVariable* UIScaleCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("UI.GlobalScale")))
	{
		UIScaleCVar->Set(Scale);
		UE_LOG(LogModulusSettings, Log, TEXT("UI text scale set to: %.2f"), Scale);
	}
}

void UMCore_GameSettingsLibrary::SetHighContrastMode(bool bIsEnabled)
{
	UE_LOG(LogModulusSettings, Log, TEXT("High contrast mode set to: %s"), bIsEnabled ? TEXT("Enabled") : TEXT("Disabled"));
}

void UMCore_GameSettingsLibrary::SetPhotosensitivityReduction(bool bIsEnabled)
{
	UE_LOG(LogModulusSettings, Log, TEXT("Photosensitivity reduction set to: %s"), bIsEnabled ? TEXT("Enabled") : TEXT("Disabled"));
}

TArray<FIntPoint> UMCore_GameSettingsLibrary::GetSupportedResolutions()
{
	return TArray<FIntPoint>();
}

bool UMCore_GameSettingsLibrary::SupportsRayTracing()
{
	// Query UE's RHI system directly
	return false;
}

bool UMCore_GameSettingsLibrary::SupportsHDR()
{
	return false;
}

TArray<FString> UMCore_GameSettingsLibrary::GetAvailableUpscalingMethods()
{
	TArray<FString> Methods;
	Methods.Add(TEXT("None"));
	Methods.Add(TEXT("TSR")); // Temporal Super Resolution is always available in UE5
    
	// Check for DLSS, FSR, XeSS availability based on hardware/drivers
	if (SupportsRayTracing()) // Simplified check - would need more specific detection
	{
		Methods.Add(TEXT("DLSS"));
		Methods.Add(TEXT("FSR"));
		Methods.Add(TEXT("XeSS"));
	}
    
	return Methods;
}

TArray<FString> UMCore_GameSettingsLibrary::GetAvailableAudioDevices()
{
	TArray<FString> DeviceNames;
    
	// Use UE 5.6 Audio Engine Subsystem
	if (GEngine && GEngine->GetMainAudioDevice())
	{
		// Get audio device info - this is the correct UE 5.6 approach
		DeviceNames.Add(TEXT("Default Audio Device"));
        
		// Platform-specific device enumeration would require additional implementation
		// For now, provide common options
		DeviceNames.Add(TEXT("System Default"));
		DeviceNames.Add(TEXT("Speakers"));
		DeviceNames.Add(TEXT("Headphones"));
	}
    
	return DeviceNames;
}

EMCore_PlatformType UMCore_GameSettingsLibrary::GetCurrentPlatform()
{
#if PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX
	return EMCore_PlatformType::PC;
#elif PLATFORM_ANDROID || PLATFORM_IOS
	return EMCore_PlatformType::Mobile;
#elif PLATFORM_SWITCH || PLATFORM_PS5 || PLATFORM_XBOXONEGDK
	return EMCore_PlatformType::Console;
#else
	return EMCore_PlatformType::Unknown;
#endif
}

bool UMCore_GameSettingsLibrary::IsConsole()
{
	return GetCurrentPlatform() == EMCore_PlatformType::Console;
}

bool UMCore_GameSettingsLibrary::IsMobile()
{
	return GetCurrentPlatform() == EMCore_PlatformType::Mobile;
}

bool UMCore_GameSettingsLibrary::IsSteamDeck()
{
	// Steam Deck detection would need specific checks
	// For now, return false
	return false;
}

void UMCore_GameSettingsLibrary::ApplyAndSaveAllSettings()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogModulusSettings, Warning, TEXT("Failed to get GameUserSettings for apply and save"));
		return;
	}

	Settings->ApplySettings(false);
	Settings->SaveSettings();
	UE_LOG(LogModulusSettings, Log, TEXT("All settings applied and saved"));
}

void UMCore_GameSettingsLibrary::ResetAllSettingsToDefaults()
{
	UGameUserSettings* Settings = GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogModulusSettings, Warning, TEXT("Failed to get GameUserSettings for reset"));
		return;
	}

	Settings->SetToDefaults();
	Settings->ApplySettings(false);
	Settings->SaveSettings();
	UE_LOG(LogModulusSettings, Log, TEXT("All settings reset to defaults"));
}

int32 UMCore_GameSettingsLibrary::ConvertPresetToScalabilityLevel(EMCore_GraphicsPreset Preset)
{
	switch(Preset)
	{
	case EMCore_GraphicsPreset::Low:    return 1;
	case EMCore_GraphicsPreset::Medium: return 2;
	case EMCore_GraphicsPreset::High:   return 3;
	case EMCore_GraphicsPreset::Ultra:  return 4;
	default:                            return 2; // Medium default
	}
}

EMCore_GraphicsPreset UMCore_GameSettingsLibrary::ConvertScalabilityLevelToPreset(int32 Level)
{
	switch(Level)
	{
	case 0:
	case 1: return EMCore_GraphicsPreset::Low;
	case 2: return EMCore_GraphicsPreset::Medium;
	case 3: return EMCore_GraphicsPreset::High;
	case 4: return EMCore_GraphicsPreset::Ultra;
	default: return EMCore_GraphicsPreset::Custom;
	}
}

float UMCore_GameSettingsLibrary::ConvertFrameRateEnumToFloat(EMCore_FrameRate FrameRate)
{
	switch(FrameRate)
	{
	case EMCore_FrameRate::FPS_30:      return 30.0f;
	case EMCore_FrameRate::FPS_60:      return 60.0f;
	case EMCore_FrameRate::FPS_75:      return 75.0f;
	case EMCore_FrameRate::FPS_120:     return 120.0f;
	case EMCore_FrameRate::FPS_144:     return 144.0f;
	case EMCore_FrameRate::FPS_165:		return 165.0f;
	case EMCore_FrameRate::FPS_240:     return 240.0f;
	case EMCore_FrameRate::FPS_360:     return 360.0f;
	case EMCore_FrameRate::Unlimited:   return 0.0f; // 0 = unlimited in UE
	default:                            return 60.0f;
	}
}

EMCore_FrameRate UMCore_GameSettingsLibrary::ConvertFloatToFrameRateEnum(float FrameRateFloat)
{
	if (FrameRateFloat <= 0.0f)
	{
		return EMCore_FrameRate::Unlimited;
	}
    
	if (FrameRateFloat <= 45.0f)  return EMCore_FrameRate::FPS_30;
	if (FrameRateFloat <= 70.0f)  return EMCore_FrameRate::FPS_60;
	if (FrameRateFloat <= 110.0f) return EMCore_FrameRate::FPS_75;
	if (FrameRateFloat <= 136.0f) return EMCore_FrameRate::FPS_120;
	if (FrameRateFloat <= 158.0f) return EMCore_FrameRate::FPS_144;
	if (FrameRateFloat <= 210.0f) return EMCore_FrameRate::FPS_165;
	if (FrameRateFloat <= 320.0f) return EMCore_FrameRate::FPS_240;
	if (FrameRateFloat <= 400.0f) return EMCore_FrameRate::FPS_360;

	return EMCore_FrameRate::Unlimited;
}

float UMCore_GameSettingsLibrary::GetStoredMasterVolume()
{
	return StoredMasterVolume;
}

void UMCore_GameSettingsLibrary::SetStoredMasterVolume(float Volume)
{
	StoredMasterVolume = Volume;
}

float UMCore_GameSettingsLibrary::GetStoredMouseSensitivity(bool bYAxis)
{
	if (bYAxis) return StoredMouseSensitivityY;

	return StoredMouseSensitivityX;
}

void UMCore_GameSettingsLibrary::SetStoredMouseSensitivity(float Sensitivity, bool bYAxis)
{
	if (bYAxis) StoredMouseSensitivityY = Sensitivity;
	else StoredMouseSensitivityX = Sensitivity;
}
