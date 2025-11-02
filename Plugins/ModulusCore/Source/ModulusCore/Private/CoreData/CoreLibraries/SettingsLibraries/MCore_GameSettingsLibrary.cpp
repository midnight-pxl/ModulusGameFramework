// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreData/CoreLibraries/SettingsLibraries/MCore_GameSettingsLibrary.h"

#include "CoreData/CoreLogging/LogModulusSettings.h"
#include "GameFramework/GameUserSettings.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/LocalPlayer.h"

static TAutoConsoleVariable<float> CVarLookSensitivityX(
	TEXT("Modulus.Input.LookSensitivityX"),
	2.0f,
	TEXT("Horizontal look sensitivity multiplier (0.1-10.0, default: 2.0)"),
	ECVF_Default
);

static TAutoConsoleVariable<float> CVarLookSensitivityY(
	TEXT("Modulus.Input.LookSensitivityY"),
	2.0f,
	TEXT("Vertical look sensitivity multiplier (0.1-10.0, default: 2.0)"),
	ECVF_Default
);

static TAutoConsoleVariable<int32> CVarInvertLookX(
	TEXT("Modulus.Input.InvertLookX"),
	0,
	TEXT("Invert horizontal look axis (0=normal, 1=inverted, default: 0)"),
	ECVF_Default
);

static TAutoConsoleVariable<int32> CVarInvertLookY(
	TEXT("Modulus.Input.InvertLookY"),
	0,
	TEXT("Invert vertical look axis (0=normal, 1=inverted, default: 0)"),
	ECVF_Default
);

void UMCore_GameSettingsLibrary::ApplyGraphicsPreset(EMCore_GraphicsPreset Preset)
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogModulusSettings, Warning, TEXT("Failed to get GameUserSettings for graphics preset"));
		return;
	}

	int32 ScalabilityLevel = ConvertPresetToScalabilityLevel(Preset);
	Settings->SetOverallScalabilityLevel(ScalabilityLevel);
	Settings->ApplySettings(false);

	UE_LOG(LogModulusSettings, Log, TEXT("MCore_GameSettingsLibrary: Applied graphics preset %s (Scalability Level: %d)"), 
		   *StaticEnum<EMCore_GraphicsPreset>()->GetNameStringByValue((int32)Preset), ScalabilityLevel);
}

EMCore_GraphicsPreset UMCore_GameSettingsLibrary::GetCurrentGraphicsPreset()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogModulusSettings, Error, TEXT("MCore_GameSettingsLibrary: Failed to get GameUserSettings"));
		return EMCore_GraphicsPreset::Medium;
	}

	const int32 CurrentScalabilityLevel = Settings->GetOverallScalabilityLevel();
	
	// Check for custom setting if other settings don't match
	const int32 AntiAliasing = Settings->GetAntiAliasingQuality();
	const int32 Shadows = Settings->GetShadowQuality();
	const int32 Textures = Settings->GetTextureQuality();
	const int32 ViewDistance = Settings->GetViewDistanceQuality();
	const int32 PostProcessing = Settings->GetPostProcessingQuality();

	if (AntiAliasing != CurrentScalabilityLevel || Shadows != CurrentScalabilityLevel ||
		Textures != CurrentScalabilityLevel || ViewDistance != CurrentScalabilityLevel ||
		PostProcessing != CurrentScalabilityLevel)
	{
		return EMCore_GraphicsPreset::Custom;
	}
	
	return ConvertScalabilityLevelToPreset(CurrentScalabilityLevel);
}

void UMCore_GameSettingsLibrary::SetLookSensitivityX(float XSensitivity)
{
	XSensitivity = FMath::Clamp(XSensitivity, 0.1f, 10.0f);
	CVarLookSensitivityX->Set(XSensitivity);

	UE_LOG(LogModulusSettings, Log, TEXT("MCore_GameSettingsLibrary: Set look sensitivity X to %.2f"), XSensitivity);
}

float UMCore_GameSettingsLibrary::GetLookSensitivityX()
{
	return CVarLookSensitivityX.GetValueOnGameThread();
}

void UMCore_GameSettingsLibrary::SetLookSensitivityY(float YSensitivity)
{
	YSensitivity = FMath::Clamp(YSensitivity, 0.1f, 10.0f);
	CVarLookSensitivityX->Set(YSensitivity);

	UE_LOG(LogModulusSettings, Log, TEXT("MCore_GameSettingsLibrary: Set look sensitivity X to %.2f"), YSensitivity);
}

float UMCore_GameSettingsLibrary::GetLookSensitivityY()
{
	return CVarLookSensitivityY.GetValueOnGameThread();
}

void UMCore_GameSettingsLibrary::SetInvertLookY(bool bInvert)
{
	CVarInvertLookY->Set(bInvert ? 1 : 0);
    
	UE_LOG(LogTemp, Log, TEXT("MCore_GameSettingsLibrary: Set invert look Y to %s"), 
		bInvert ? TEXT("true") : TEXT("false"));
}

bool UMCore_GameSettingsLibrary::GetInvertLookY()
{
	return CVarInvertLookY.GetValueOnGameThread() ? true : false;
}

void UMCore_GameSettingsLibrary::SetInvertLookX(bool bInvert)
{
	CVarInvertLookX->Set(bInvert ? 1 : 0);
    
	UE_LOG(LogTemp, Log, TEXT("MCore_GameSettingsLibrary: Set invert look X to %s"), 
		bInvert ? TEXT("true") : TEXT("false"));
}

bool UMCore_GameSettingsLibrary::GetInvertLookX()
{
	return CVarInvertLookX.GetValueOnGameThread() ? true : false;
}

int32 UMCore_GameSettingsLibrary::ConvertPresetToScalabilityLevel(EMCore_GraphicsPreset Preset)
{
	switch (Preset)
	{
	case EMCore_GraphicsPreset::Low:    return 0;
	case EMCore_GraphicsPreset::Medium: return 1;
	case EMCore_GraphicsPreset::High:   return 2;
	case EMCore_GraphicsPreset::Ultra:  return 3;
	case EMCore_GraphicsPreset::Custom:
		{
			UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
			return Settings ? Settings->GetOverallScalabilityLevel() : 1;
		}
	default:
		UE_LOG(LogModulusSettings, Warning, TEXT("Unknown preset, defaulting to Medium"));
		return 1;
	}
}

EMCore_GraphicsPreset UMCore_GameSettingsLibrary::ConvertScalabilityLevelToPreset(int32 Level)
{
	switch (Level)
	{
	case 0:  return EMCore_GraphicsPreset::Low;
	case 1:  return EMCore_GraphicsPreset::Medium;
	case 2:  return EMCore_GraphicsPreset::High;
	case 3:
	case 4:  return EMCore_GraphicsPreset::Ultra; // Cinematic maps to Ultra
	default:
		UE_LOG(LogModulusSettings, Warning, TEXT("Invalid scalability level %d, defaulting to Medium"), Level);
		return EMCore_GraphicsPreset::Medium;
	}
}
