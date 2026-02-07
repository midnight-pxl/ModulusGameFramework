// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreData/Libraries/MCore_GameSettingsLibrary.h"

#include "CoreData/Logging/LogModulusSettings.h"
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

float UMCore_GameSettingsLibrary::GetSettingFloat(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting)
{
	return 0.0f;
}

int32 UMCore_GameSettingsLibrary::GetSettingInt(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting)
{
	return 0;
}

bool UMCore_GameSettingsLibrary::GetSettingBool(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting)
{
	return false;
}

FKey UMCore_GameSettingsLibrary::GetSettingKey(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting)
{
	return FKey{};
}

void UMCore_GameSettingsLibrary::SetSettingFloat(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting, float Value)
{
}

void UMCore_GameSettingsLibrary::SetSettingInt(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting, int32 Value)
{
}

void UMCore_GameSettingsLibrary::SetSettingBool(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting, bool Value)
{
}

void UMCore_GameSettingsLibrary::SetSettingKey(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting, const FKey& Value)
{
}

void UMCore_GameSettingsLibrary::ApplyPendingSettings(const UObject* WorldContextObject,
	const UMCore_DA_SettingsCollection* Collection)
{
}

void UMCore_GameSettingsLibrary::DiscardPendingSettings(const UObject* WorldContextObject,
	const UMCore_DA_SettingsCollection* Collection)
{
}

void UMCore_GameSettingsLibrary::PreviewPendingSettings(const UObject* WorldContextObject,
	const UMCore_DA_SettingsCollection* Collection)
{
}

void UMCore_GameSettingsLibrary::ResetSettingToDefault(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting)
{
}

void UMCore_GameSettingsLibrary::SavePlayerSettings(const UObject* WorldContextObject)
{
}

void UMCore_GameSettingsLibrary::ApplySettingToEngine(const UMCore_DA_SettingDefinition* Setting, float FloatValue,
	int32 IntValue, bool BoolValue)
{
}

void UMCore_GameSettingsLibrary::ApplyToGameUserSettings(const FName& PropertyName, float Value)
{
}

void UMCore_GameSettingsLibrary::ApplyToGameUserSettings(const FName& PropertyName, int32 Value)
{
}

void UMCore_GameSettingsLibrary::ApplyToGameUserSettings(const FName& PropertyName, bool Value)
{
}

void UMCore_GameSettingsLibrary::ApplyToConsoleVariable(const FName& CVarName, float Value)
{
}

void UMCore_GameSettingsLibrary::ApplyToConsoleVariable(const FName& CVarName, int32 Value)
{
}

void UMCore_GameSettingsLibrary::ApplyToConsoleVariable(const FName& CVarName, bool Value)
{
}

void UMCore_GameSettingsLibrary::ApplyToSoundClass(const TSoftObjectPtr<USoundClass>& SoundClassRef, float Volume)
{
}

void UMCore_GameSettingsLibrary::BroadcastSettingChanged(const UObject* WorldContextObject,
	const FGameplayTag& SettingTag)
{
}

UMCore_PlayerSettingsSave* UMCore_GameSettingsLibrary::GetPlayerSave(const UObject* WorldContextObject)
{
	return nullptr;
}

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

	/** Check for custom setting if other settings don't match */
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
	case 4:  return EMCore_GraphicsPreset::Ultra; /** Cinematic maps to Ultra */
	default:
		UE_LOG(LogModulusSettings, Warning, TEXT("Invalid scalability level %d, defaulting to Medium"), Level);
		return EMCore_GraphicsPreset::Medium;
	}
}
