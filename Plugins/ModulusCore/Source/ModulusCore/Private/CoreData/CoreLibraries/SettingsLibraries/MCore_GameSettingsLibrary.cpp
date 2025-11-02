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

void UMCore_GameSettingsLibrary::SetLookSensitivityX(float XSensitivity)
{
}

float UMCore_GameSettingsLibrary::GetLookSensitivityX(float XSensitivity)
{
}

void UMCore_GameSettingsLibrary::SetLookSensitivityY(float YSensitivity)
{
}

float UMCore_GameSettingsLibrary::GetLookSensitivityY(float YSensitivity)
{
}

void UMCore_GameSettingsLibrary::SetInvertLookY(bool bInvert)
{
}

bool UMCore_GameSettingsLibrary::GetInvertLookY()
{
}

void UMCore_GameSettingsLibrary::SetInvertLookX(bool bInvert)
{
}

bool UMCore_GameSettingsLibrary::GetInvertLookX()
{
}