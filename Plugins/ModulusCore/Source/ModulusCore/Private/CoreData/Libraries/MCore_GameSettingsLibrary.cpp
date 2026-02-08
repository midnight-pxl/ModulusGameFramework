// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreData/Libraries/MCore_GameSettingsLibrary.h"

#include "CoreUI/MCore_UISubsystem.h"
#include "CoreData/Logging/LogModulusSettings.h"
#include "CoreUI/Settings/MCore_PlayerSettingsSave.h"
#include "CoreData/Libraries/MCore_EventFunctionLibrary.h"
#include "CoreData/Types/Settings/MCore_DA_SettingDefinition.h"
#include "CoreData/Types/Settings/MCore_DA_SettingsCollection.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Sound/SoundClass.h"
#include "Engine/LocalPlayer.h"
#include "HAL/IConsoleManager.h"
#include "GameFramework/GameUserSettings.h"

// ============================================================================
// CONSOLE VARIABLES (registered DataAssets target)
// ============================================================================

static TAutoConsoleVariable<float> CVarMouseSensitivity(
	TEXT("Modulus.Input.MouseSensitivity"),
	1.0f,
	TEXT("Overall mouse sensitivity multiplier (0.1-10.0, default: 1.0)"),
	ECVF_Default
	);

static TAutoConsoleVariable<float> CVarGamepadSensitivity(
	TEXT("Modulus.Input.GamepadSensitivity"),
	1.0f,
	TEXT("Overall gamepad sensitivity multiplier (0.1-10.0, default: 1.0)"),
	ECVF_Default
	);

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
	1,
	TEXT("Invert vertical look axis (0=normal, 1=inverted, default: 1)"),
	ECVF_Default
	);

static TAutoConsoleVariable<int32> CVarVibration(
	TEXT("Modulus.Input.Vibration"),
	1,
	TEXT("Controller vibration (0=off, 1=on)"),
	ECVF_Default
	);

static TAutoConsoleVariable<int32> CVarMouseSmoothing(
	TEXT("Modulus.Input.MouseSmoothing"),
	0,
	TEXT("Mouse smoothing (0=off, 1=on)"),
	ECVF_Default
	);

static TAutoConsoleVariable<int32> CVarSubtitles(
	TEXT("Modulus.Audio.Subtitles"),
	1,
	TEXT("Show subtitles (0=off, 1=on)"),
	ECVF_Default
	);

static TAutoConsoleVariable<int32> CVarScreenReader(
	TEXT("Modulus.Accessibility.ScreenReader"), 
	0,
	TEXT("Screen reader support (0=off, 1=on)"), 
	ECVF_Default
	);

static TAutoConsoleVariable<int32> CVarLargeText(
	TEXT("Modulus.Accessibility.LargeText"),
	0,
	TEXT("Large text mode (0=off, 1=on)"),
	ECVF_Default
	);

// ============================================================================
// INTERNAL HELPER
// ============================================================================

UMCore_PlayerSettingsSave* UMCore_GameSettingsLibrary::GetPlayerSave(const UObject* WorldContextObject)
{
	if (!WorldContextObject) { return nullptr; }
	
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,
		EGetWorldErrorMode::LogAndReturnNull);
	if (!World) { return nullptr; }
	
	const ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController();
	if (!LocalPlayer) { return nullptr; }
	
	UMCore_UISubsystem* UISubsystem = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
	if (!UISubsystem) { return nullptr; }
	
	return UISubsystem->GetPlayerSettings();
}

// ============================================================================
// TYPED GETTERS (pending → committed → DataAsset default)
// ============================================================================

float UMCore_GameSettingsLibrary::GetSettingFloat(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting)
{
	if (!Setting) { return 0.0f; }
	
	if (Setting->SettingType != EMCore_SettingType::Slider)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GetSettingFloat: called on non-Slider setting '%s'"),
			*Setting->SettingTag.ToString());
	}
	
	const UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (Save)
	{
		const FString Key = Setting->GetSaveKey();
		
		if (const float* TempSetting = Save->PendingFloatSettings.Find(Key)) { return *TempSetting; }
		
		float CommitSetting;
		if (Save->GetFloatSetting(Key, CommitSetting)) { return CommitSetting; }
	}
	
	return Setting->DefaultValue;
}

int32 UMCore_GameSettingsLibrary::GetSettingInt(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting)
{
	if (!Setting) { return 0; }
	
	if (Setting->SettingType != EMCore_SettingType::Dropdown)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GetSettingInt: called on non-Dropdown setting '%s'"),
			*Setting->SettingTag.ToString());
	}
	
	const UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (Save)
	{
		const FString Key = Setting->GetSaveKey();
		
		if (const int32* TempSetting = Save->PendingIntSettings.Find(Key)) { return *TempSetting; }
		
		int32 CommitSetting;
		if (Save->GetIntSetting(Key, CommitSetting)) { return CommitSetting; }
	}
	
	return Setting->DefaultDropdownIndex;
}

bool UMCore_GameSettingsLibrary::GetSettingBool(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting)
{
	if (!Setting) { return false; }
	
	if (Setting->SettingType != EMCore_SettingType::Toggle)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GetSettingBool: called on non-Toggle setting '%s'"),
			*Setting->SettingTag.ToString());
	}
	
	const UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (Save)
	{
		const FString Key = Setting->GetSaveKey();
		
		if (const bool* TempSetting = Save->PendingBoolSettings.Find(Key)) { return *TempSetting; }
		
		bool CommitSetting;
		if (Save->GetBoolSetting(Key, CommitSetting)) { return CommitSetting; }
	}
	
	return Setting->DefaultToggleValue;
}

FKey UMCore_GameSettingsLibrary::GetSettingKey(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting)
{
	if (!Setting) { return EKeys::Invalid; }
	
	if (Setting->SettingType != EMCore_SettingType::KeyBinding)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GetSettingKey: called on non-KeyBinding setting '%s'"),
			*Setting->SettingTag.ToString());
	}
	
	const UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (Save)
	{
		const FString Key = Setting->GetSaveKey();
		
		if (const FKey* TempSetting = Save->PendingKeySettings.Find(Key)) { return *TempSetting; }
		
		FKey CommitSetting;
		if (Save->GetKeySetting(Key, CommitSetting)) { return CommitSetting; }
	}
	
	return Setting->DefaultKey;
}

// ============================================================================
// TYPED SETTERS (stage pending)
// ============================================================================

void UMCore_GameSettingsLibrary::SetSettingFloat(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting, float Value)
{
	if (!Setting) { return; }

	UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (!Save) { return; }

	/** Clamp to DataAsset-defined range */
	if (Setting->SettingType == EMCore_SettingType::Slider)
	{
		Value = FMath::Clamp(Value, Setting->MinValue, Setting->MaxValue);
	}

	Save->SetPendingFloat(Setting->GetSaveKey(), Value);
}

void UMCore_GameSettingsLibrary::SetSettingInt(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting, int32 Value)
{
	if (!Setting) { return; }

	UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (!Save) { return; }

	/** Clamp to valid dropdown range */
	if (Setting->SettingType == EMCore_SettingType::Dropdown && Setting->DropdownOptions.Num() > 0)
	{
		Value = FMath::Clamp(Value, 0, Setting->DropdownOptions.Num() - 1);
	}

	Save->SetPendingInt(Setting->GetSaveKey(), Value);
}

void UMCore_GameSettingsLibrary::SetSettingBool(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting, bool Value)
{
	if (!Setting) { return; }

	UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (!Save) { return; }

	Save->SetPendingBool(Setting->GetSaveKey(), Value);
}

void UMCore_GameSettingsLibrary::SetSettingKey(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting, const FKey& Value)
{
	if (!Setting) { return; }

	UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (!Save) { return; }

	Save->SetPendingKey(Setting->GetSaveKey(), Value);
}

// ============================================================================
// DEFERRED-APPLY LIFECYCLE
// ============================================================================

void UMCore_GameSettingsLibrary::ApplyPendingSettings(const UObject* WorldContextObject,
	const UMCore_DA_SettingsCollection* Collection)
{
	if (!Collection) { return; }

	UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (!Save) { return; }

	bool bSettingsDirty = false;

	/** Iterate collection and apply each pending change to engine targets */
	for (const UMCore_DA_SettingDefinition* Setting : Collection->GetAllSettings())
	{
		if (!Setting) { continue; }

		const FString Key = Setting->GetSaveKey();
		bool bApplied = false;

		switch (Setting->SettingType)
		{
		case EMCore_SettingType::Slider:
			if (const float* SliderValue = Save->PendingFloatSettings.Find(Key))
			{
				ApplySettingToEngine(Setting, *SliderValue, 0, false);
				bApplied = true;
				if (!Setting->GameUserSettingsProperty.IsNone()) { bSettingsDirty = true; }
			}
			break;

		case EMCore_SettingType::Toggle:
			if (const bool* ToggleValue = Save->PendingBoolSettings.Find(Key))
			{
				ApplySettingToEngine(Setting, 0.0f, 0, *ToggleValue);
				bApplied = true;
				if (!Setting->GameUserSettingsProperty.IsNone()) { bSettingsDirty = true; }
			}
			break;

		case EMCore_SettingType::Dropdown:
			if (const int32* DropValue = Save->PendingIntSettings.Find(Key))
			{
				ApplySettingToEngine(Setting, 0.0f, *DropValue, false);
				bApplied = true;
				if (!Setting->GameUserSettingsProperty.IsNone()) { bSettingsDirty = true; }
			}
			break;

		default:
			break;
		}

		if (bApplied)
		{
			BroadcastSettingChanged(WorldContextObject, Setting->SettingTag);
		}
	}

	/** Commit all pending to saved state */
	Save->CommitPendingSettings();

	/** Push GameUserSettings to disk if any properties were written */
	if (bSettingsDirty)
	{
		if (UGameUserSettings* GameSettings = UGameUserSettings::GetGameUserSettings())
		{
			GameSettings->ApplySettings(false);
		}
	}

	/** Persist to save file */
	Save->SaveSettings();

	UE_LOG(LogModulusSettings, Log, TEXT("Pending settings applied and saved"));
}

void UMCore_GameSettingsLibrary::DiscardPendingSettings(const UObject* WorldContextObject,
	const UMCore_DA_SettingsCollection* Collection)
{
	if (!Collection) { return; }

	UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (!Save) { return; }

	if (!Save->HasPendingChanges()) { return; }

	bool bSettingsDirty = false;

	/** Re-apply committed values to revert any previewed engine state */
	for (const UMCore_DA_SettingDefinition* Setting : Collection->GetAllSettings())
	{
		if (!Setting) { continue; }

		const FString Key = Setting->GetSaveKey();

		switch (Setting->SettingType)
		{
		case EMCore_SettingType::Slider:
			if (Save->PendingFloatSettings.Contains(Key))
			{
				float CommitSetting = Setting->DefaultValue;
				Save->GetFloatSetting(Key, CommitSetting);
				ApplySettingToEngine(Setting, CommitSetting, 0, false);
				if (!Setting->GameUserSettingsProperty.IsNone()) { bSettingsDirty = true; }
			}
			break;

		case EMCore_SettingType::Toggle:
			if (Save->PendingBoolSettings.Contains(Key))
			{
				bool CommitSetting = Setting->DefaultToggleValue;
				Save->GetBoolSetting(Key, CommitSetting);
				ApplySettingToEngine(Setting, 0.0f, 0, CommitSetting);
				if (!Setting->GameUserSettingsProperty.IsNone()) { bSettingsDirty = true; }
			}
			break;

		case EMCore_SettingType::Dropdown:
			if (Save->PendingIntSettings.Contains(Key))
			{
				int32 CommitSetting = Setting->DefaultDropdownIndex;
				Save->GetIntSetting(Key, CommitSetting);
				ApplySettingToEngine(Setting, 0.0f, CommitSetting, false);
				if (!Setting->GameUserSettingsProperty.IsNone()) { bSettingsDirty = true; }
			}
			break;

		default:
			break;
		}
	}

	/** Clear pending settings */
	Save->DiscardPendingSettings();

	if (bSettingsDirty)
	{
		if (UGameUserSettings* GameSettings = UGameUserSettings::GetGameUserSettings())
		{
			GameSettings->ApplySettings(false);
		}
	}

	UE_LOG(LogModulusSettings, Log, TEXT("Pending settings discarded"));
}

void UMCore_GameSettingsLibrary::PreviewPendingSettings(const UObject* WorldContextObject,
	const UMCore_DA_SettingsCollection* Collection)
{
	if (!Collection) { return; }

	const UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (!Save || !Save->HasPendingChanges()) { return; }

	bool bSettingsDirty = false;

	for (const UMCore_DA_SettingDefinition* Setting : Collection->GetAllSettings())
	{
		if (!Setting) { continue; }

		const FString Key = Setting->GetSaveKey();

		switch (Setting->SettingType)
		{
		case EMCore_SettingType::Slider:
			if (const float* SliderValue = Save->PendingFloatSettings.Find(Key))
			{
				ApplySettingToEngine(Setting, *SliderValue, 0, false);
				if (!Setting->GameUserSettingsProperty.IsNone()) { bSettingsDirty = true; }
			}
			break;

		case EMCore_SettingType::Toggle:
			if (const bool* ToggleValue = Save->PendingBoolSettings.Find(Key))
			{
				ApplySettingToEngine(Setting, 0.0f, 0, *ToggleValue);
				if (!Setting->GameUserSettingsProperty.IsNone()) { bSettingsDirty = true; }
			}
			break;

		case EMCore_SettingType::Dropdown:
			if (const int32* DropValue = Save->PendingIntSettings.Find(Key))
			{
				ApplySettingToEngine(Setting, 0.0f, *DropValue, false);
				if (!Setting->GameUserSettingsProperty.IsNone()) { bSettingsDirty = true; }
			}
			break;

		default:
			break;
		}
	}

	if (bSettingsDirty)
	{
		if (UGameUserSettings* GameSettings = UGameUserSettings::GetGameUserSettings())
		{
			GameSettings->ApplySettings(false);
		}
	}
}

void UMCore_GameSettingsLibrary::ResetSettingToDefault(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting)
{
	if (!Setting) { return; }

	UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (!Save) { return; }

	const FString Key = Setting->GetSaveKey();

	switch (Setting->SettingType)
	{
	case EMCore_SettingType::Slider:
		Save->SetPendingFloat(Key, Setting->DefaultValue);
		break;
	case EMCore_SettingType::Toggle:
		Save->SetPendingBool(Key, Setting->DefaultToggleValue);
		break;
	case EMCore_SettingType::Dropdown:
		Save->SetPendingInt(Key, Setting->DefaultDropdownIndex);
		break;
	case EMCore_SettingType::KeyBinding:
		Save->SetPendingKey(Key, Setting->DefaultKey);
		break;
	default:
		break;
	}
}

void UMCore_GameSettingsLibrary::SavePlayerSettings(const UObject* WorldContextObject)
{
	if (UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject))
	{
		Save->SaveSettings();
	}
}

// ============================================================================
// ENGINE APPLY DISPATCHER
// ============================================================================

void UMCore_GameSettingsLibrary::ApplySettingToEngine(const UMCore_DA_SettingDefinition* Setting, float FloatValue,
	int32 IntValue, bool BoolValue)
{
	if (!Setting) { return; }

	/** GameUserSettings property (FProperty reflection) */
	if (!Setting->GameUserSettingsProperty.IsNone())
	{
		switch (Setting->SettingType)
		{
		case EMCore_SettingType::Slider:
			ApplyToGameUserSettings(Setting->GameUserSettingsProperty, FloatValue);
			break;
		case EMCore_SettingType::Toggle:
			ApplyToGameUserSettings(Setting->GameUserSettingsProperty, BoolValue);
			break;
		case EMCore_SettingType::Dropdown:
			ApplyToGameUserSettings(Setting->GameUserSettingsProperty, IntValue);
			break;
		default:
			break;
		}
	}

	/** Console variable */
	if (!Setting->ConsoleVariable.IsNone())
	{
		switch (Setting->SettingType)
		{
		case EMCore_SettingType::Slider:
			ApplyToConsoleVariable(Setting->ConsoleVariable, FloatValue);
			break;
		case EMCore_SettingType::Toggle:
			ApplyToConsoleVariable(Setting->ConsoleVariable, BoolValue);
			break;
		case EMCore_SettingType::Dropdown:
			ApplyToConsoleVariable(Setting->ConsoleVariable, IntValue);
			break;
		default:
			break;
		}
	}

	/** SoundClass volume (Slider only) */
	if (!Setting->SoundClass.IsNull() && Setting->SettingType == EMCore_SettingType::Slider)
	{
		ApplyToSoundClass(Setting->SoundClass, FloatValue);
	}
}

// ============================================================================
// GAME USER SETTINGS (FProperty reflection)
// ============================================================================

/** float override */
void UMCore_GameSettingsLibrary::ApplyToGameUserSettings(const FName& PropertyName, float Value)
{
	UGameUserSettings* GameSettings = UGameUserSettings::GetGameUserSettings();
	if (!GameSettings) { return; }

	FProperty* ThisProperty = FindFProperty<FProperty>(GameSettings->GetClass(), PropertyName);
	if (!ThisProperty)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSetting property '%s' not found"), *PropertyName.ToString());
		return;
	}

	if (FFloatProperty* FloatProp = CastField<FFloatProperty>(ThisProperty))
	{
		FloatProp->SetPropertyValue_InContainer(GameSettings, Value);
	}
	else if (FDoubleProperty* DoubleProp = CastField<FDoubleProperty>(ThisProperty))
	{
		DoubleProp->SetPropertyValue_InContainer(GameSettings, static_cast<double>(Value));
	}
	else
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSetting property '%s' is not float/double"), *PropertyName.ToString());
	}
}
/** int override */
void UMCore_GameSettingsLibrary::ApplyToGameUserSettings(const FName& PropertyName, int32 Value)
{
	UGameUserSettings* GameSettings = UGameUserSettings::GetGameUserSettings();
	if (!GameSettings) { return; }

	FProperty* ThisProperty = FindFProperty<FProperty>(GameSettings->GetClass(), PropertyName);
	if (!ThisProperty)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSetting property '%s' not found"), *PropertyName.ToString());
		return;
	}

	if (FIntProperty* IntProp = CastField<FIntProperty>(ThisProperty))
	{
		IntProp->SetPropertyValue_InContainer(GameSettings, Value);
	}
	else
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSetting property '%s' is not int32"), *PropertyName.ToString());
	}
}
/** bool override */
void UMCore_GameSettingsLibrary::ApplyToGameUserSettings(const FName& PropertyName, bool Value)
{
	UGameUserSettings* GameSettings = UGameUserSettings::GetGameUserSettings();
	if (!GameSettings) { return; }

	FProperty* ThisProperty = FindFProperty<FProperty>(GameSettings->GetClass(), PropertyName);
	if (!ThisProperty)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSetting property '%s' not found"), *PropertyName.ToString());
		return;
	}

	if (FBoolProperty* BoolProp = CastField<FBoolProperty>(ThisProperty))
	{
		BoolProp->SetPropertyValue_InContainer(GameSettings, Value);
	}
	else
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSetting property '%s' is not bool"), *PropertyName.ToString());
	}
}

// ============================================================================
// CONSOLE VARIABLES
// ============================================================================

/** float override */
void UMCore_GameSettingsLibrary::ApplyToConsoleVariable(const FName& CVarName, float Value)
{
	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(*CVarName.ToString());
	if (CVar)
	{
		CVar->Set(Value, ECVF_SetByCode);
	}
	else
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("Console variable '%s' not found"), *CVarName.ToString());
	}
}
/** int override */
void UMCore_GameSettingsLibrary::ApplyToConsoleVariable(const FName& CVarName, int32 Value)
{
	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(*CVarName.ToString());
	if (CVar)
	{
		CVar->Set(Value, ECVF_SetByCode);
	}
	else
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("Console variable '%s' not found"), *CVarName.ToString());
	}
}
/** bool override */
void UMCore_GameSettingsLibrary::ApplyToConsoleVariable(const FName& CVarName, bool Value)
{
	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(*CVarName.ToString());
	if (CVar)
	{
		CVar->Set(Value, ECVF_SetByCode);
	}
	else
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("Console variable '%s' not found"), *CVarName.ToString());
	}
}

// ============================================================================
// SOUND CLASS
// ============================================================================

void UMCore_GameSettingsLibrary::ApplyToSoundClass(const TSoftObjectPtr<USoundClass>& SoundClassRef, float Volume)
{
	USoundClass* LoadedClass = SoundClassRef.LoadSynchronous();
	if (!LoadedClass)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("Failed to load SoundClass: %s"), *SoundClassRef.ToString());
		return;
	}

	LoadedClass->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);

	UE_LOG(LogModulusSettings, Verbose,
		TEXT("SoundClass '%s' volume set to %.2f"),
		*LoadedClass->GetName(), Volume);
}

// ============================================================================
// EVENT BROADCASTING
// ============================================================================

void UMCore_GameSettingsLibrary::BroadcastSettingChanged(const UObject* WorldContextObject,
	const FGameplayTag& SettingTag)
{
	if (!SettingTag.IsValid() || !WorldContextObject) { return; }
	
	UMCore_EventFunctionLibrary::BroadcastSimpleEvent(WorldContextObject,
		SettingTag, EMCore_EventScope::Local);
}

// ============================================================================
// GRAPHICS PRESETS
// ============================================================================

void UMCore_GameSettingsLibrary::ApplyGraphicsPreset(EMCore_GraphicsPreset Preset)
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogModulusSettings, Warning, TEXT("Failed to get GameUserSettings for graphics preset"));
		return;
	}

	const int32 ScalabilityLevel = ConvertPresetToScalabilityLevel(Preset);
	Settings->SetOverallScalabilityLevel(ScalabilityLevel);
	Settings->ApplySettings(false);

	UE_LOG(LogModulusSettings, Log,
		TEXT("MCore_GameSettingsLibrary: Applied graphics preset %s (Scalability Level: %d)"),
		*StaticEnum<EMCore_GraphicsPreset>()->GetNameStringByValue((int32)Preset), ScalabilityLevel);
}

EMCore_GraphicsPreset UMCore_GameSettingsLibrary::GetCurrentGraphicsPreset()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogModulusSettings, Error,
			TEXT("MCore_GameSettingsLibrary: Failed to get GameUserSettings"));
		return EMCore_GraphicsPreset::Medium;
	}

	const int32 CurrentLevel = Settings->GetOverallScalabilityLevel();

	/** Check for custom setting if other settings don't match */
	const int32 AntiAliasing = Settings->GetAntiAliasingQuality();
	const int32 Shadows = Settings->GetShadowQuality();
	const int32 Textures = Settings->GetTextureQuality();
	const int32 ViewDistance = Settings->GetViewDistanceQuality();
	const int32 PostProcessing = Settings->GetPostProcessingQuality();

	if (AntiAliasing != CurrentLevel
		|| Shadows != CurrentLevel
		|| Textures != CurrentLevel
		|| ViewDistance != CurrentLevel
		|| PostProcessing != CurrentLevel)
	{
		return EMCore_GraphicsPreset::Custom;
	}
	
	return ConvertScalabilityLevelToPreset(CurrentLevel);
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
			const UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
			return Settings ? Settings->GetOverallScalabilityLevel() : 1;
		}
	default:
		UE_LOG(LogModulusSettings, Warning,
			TEXT("Unknown preset, defaulting to Medium"));
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
		UE_LOG(LogModulusSettings, Warning,
			TEXT("Invalid scalability level %d, defaulting to Medium"), Level);
		return EMCore_GraphicsPreset::Medium;
	}
}
