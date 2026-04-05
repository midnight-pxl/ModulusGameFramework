// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreData/Libraries/MCore_GameSettingsLibrary.h"

#include "CoreUI/MCore_UISubsystem.h"
#include "CoreData/Logging/LogModulusSettings.h"
#include "CoreData/Types/Settings/MCore_PlayerSettingsSave.h"
#include "CoreData/Libraries/MCore_EventFunctionLibrary.h"
#include "CoreData/Types/Settings/MCore_DA_SettingDefinition.h"
#include "CoreData/Types/Settings/MCore_DA_SettingsCollection.h"
#include "CoreData/Tags/MCore_SettingsTags.h"
#include "CoreData/DevSettings/MCore_CoreSettings.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Sound/SoundClass.h"
#include "Engine/LocalPlayer.h"
#include "HAL/IConsoleManager.h"
#include "GameFramework/GameUserSettings.h"

FOnSettingsConfirmationRequired UMCore_GameSettingsLibrary::OnSettingsConfirmationRequired;

// ============================================================================
// CONSOLE VARIABLES
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
// TYPED GETTERS
// ============================================================================

float UMCore_GameSettingsLibrary::GetSettingFloat(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting)
{
	if (!Setting) { return 0.0f; }

	if (Setting->SettingType != EMCore_SettingType::Slider)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSettingsLibrary::GetSettingFloat -- called on non-Slider setting '%s'"),
			*Setting->SettingTag.ToString());
	}

	if (const UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject))
	{
		float StoredValue = 0.f;
		if (Save->GetFloatSetting(Setting->GetSaveKey(), StoredValue))
		{
			return StoredValue;
		}
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
			TEXT("GameSettingsLibrary::GetSettingInt -- called on non-Dropdown setting '%s'"),
			*Setting->SettingTag.ToString());
	}

	if (const UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject))
	{
		int32 StoredValue = 0;
		if (Save->GetIntSetting(Setting->GetSaveKey(), StoredValue))
		{
			return StoredValue;
		}
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
			TEXT("GameSettingsLibrary::GetSettingBool -- called on non-Toggle setting '%s'"),
			*Setting->SettingTag.ToString());
	}

	if (const UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject))
	{
		bool StoredValue = false;
		if (Save->GetBoolSetting(Setting->GetSaveKey(), StoredValue))
		{
			return StoredValue;
		}
	}

	return Setting->DefaultToggleValue;
}

// ============================================================================
// TEMPLATE HELPERS (private static, defined here, declared in header)
// ============================================================================

template<typename TChangeStruct, typename TValue>
void UMCore_GameSettingsLibrary::ApplySettingChanges_Internal(
	const UObject* WorldContextObject,
	const TArray<TChangeStruct>& Changes,
	bool bBypassConfirmation,
	TFunctionRef<TValue(const UMCore_DA_SettingDefinition*, TValue)> ClampValue,
	TFunctionRef<void(UMCore_PlayerSettingsSave*, const FString&, TValue)> SetCommitted,
	TFunctionRef<void(const UMCore_DA_SettingDefinition*, TValue)> ApplyToEngine)
{
	if (!WorldContextObject || Changes.IsEmpty()) { return; }

	UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (!Save)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSettingsLibrary::ApplySettingChanges -- failed to get PlayerSettingsSave"));
		return;
	}

	TArray<FGameplayTag> AffectedTags;
	TArray<FGameplayTag> ProcessedTags;
	bool bAnyRequiresConfirmation{false};

	for (const TChangeStruct& Change : Changes)
	{
		if (!Change.Setting)
		{
			UE_LOG(LogModulusSettings, Warning,
				TEXT("GameSettingsLibrary::ApplySettingChanges -- null Setting in Changes, skipping"));
			continue;
		}

		const FString Key = Change.Setting->GetSaveKey();
		const TValue ClampedVal = ClampValue(Change.Setting, Change.Value);

		SetCommitted(Save, Key, ClampedVal);
		ApplyToEngine(Change.Setting, ClampedVal);

		if (Change.Setting->bRequiresConfirmation && !bBypassConfirmation)
		{
			bAnyRequiresConfirmation = true;
			AffectedTags.Add(Change.Setting->SettingTag);
		}
		else
		{
			ProcessedTags.Add(Change.Setting->SettingTag);
		}
	}

	if (UGameUserSettings* GUS = UGameUserSettings::GetGameUserSettings())
	{
		GUS->ApplySettings(false);
	}

	if (bAnyRequiresConfirmation)
	{
		/* Stringify tags for the event broadcast payload */
		TArray<FString> AffectedTagStrings;
		AffectedTagStrings.Reserve(AffectedTags.Num());
		for (const FGameplayTag& Tag : AffectedTags)
		{
			AffectedTagStrings.Add(Tag.ToString());
		}

		TMap<FString, FString> EventParams;
		EventParams.Add(TEXT("SettingTags"), FString::Join(AffectedTagStrings, TEXT("|")));

		UMCore_EventFunctionLibrary::BroadcastEvent(
			WorldContextObject,
			MCore_SettingsTags::MCore_Settings_Event_ConfirmationRequired,
			EventParams, EMCore_EventScope::Local);

		OnSettingsConfirmationRequired.Broadcast(AffectedTags);
	}
	else
	{
		SavePlayerSettings(WorldContextObject);
	}

	if (!bBypassConfirmation)
	{
		for (const FGameplayTag& Tag : ProcessedTags)
		{
			BroadcastSettingChanged(WorldContextObject, Tag);
		}
	}
}

template<typename TValue>
TValue UMCore_GameSettingsLibrary::GetSettingByTag_Internal(
	const UObject* WorldContextObject,
	const FGameplayTag& SettingTag,
	TFunctionRef<TValue(const UObject*, const UMCore_DA_SettingDefinition*)> TypedGetter,
	TValue DefaultReturn,
	const TCHAR* FunctionName)
{
	const UMCore_DA_SettingDefinition* Setting = UMCore_CoreSettings::Get()->FindSettingDefinitionByTag(SettingTag);
	if (!Setting)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSettingsLibrary::%s -- no definition found for tag '%s' in any collection"),
			FunctionName, *SettingTag.ToString());
		return DefaultReturn;
	}
	return TypedGetter(WorldContextObject, Setting);
}

// ============================================================================
// TAG-BASED GETTERS
// ============================================================================

float UMCore_GameSettingsLibrary::GetSettingFloatByTag(const UObject* WorldContextObject,
	FGameplayTag SettingTag)
{
	return GetSettingByTag_Internal<float>(
		WorldContextObject, SettingTag,
		[](const UObject* WCO, const UMCore_DA_SettingDefinition* S) { return GetSettingFloat(WCO, S); },
		0.f, TEXT("GetSettingFloatByTag"));
}

int32 UMCore_GameSettingsLibrary::GetSettingIntByTag(const UObject* WorldContextObject,
	FGameplayTag SettingTag)
{
	return GetSettingByTag_Internal<int32>(
		WorldContextObject, SettingTag,
		[](const UObject* WCO, const UMCore_DA_SettingDefinition* S) { return GetSettingInt(WCO, S); },
		0, TEXT("GetSettingIntByTag"));
}

bool UMCore_GameSettingsLibrary::GetSettingBoolByTag(const UObject* WorldContextObject,
	FGameplayTag SettingTag)
{
	return GetSettingByTag_Internal<bool>(
		WorldContextObject, SettingTag,
		[](const UObject* WCO, const UMCore_DA_SettingDefinition* S) { return GetSettingBool(WCO, S); },
		false, TEXT("GetSettingBoolByTag"));
}

// ============================================================================
// TYPED SETTERS
// ============================================================================

void UMCore_GameSettingsLibrary::SetSettingFloat(
	const UObject* WorldContextObject,
	const TArray<FMCore_FloatSettingChange>& Changes,
	bool bBypassConfirmation)
{
	ApplySettingChanges_Internal<FMCore_FloatSettingChange, float>(
		WorldContextObject, Changes, bBypassConfirmation,
		[](const UMCore_DA_SettingDefinition* S, float V) {
			return (S->SettingType == EMCore_SettingType::Slider)
				? FMath::Clamp(V, S->MinValue, S->MaxValue) : V;
		},
		[](UMCore_PlayerSettingsSave* Save, const FString& Key, float V) { Save->SetFloatSetting(Key, V); },
		[](const UMCore_DA_SettingDefinition* S, float V) { ApplySettingToEngine(S, V, 0, false); });
}

void UMCore_GameSettingsLibrary::SetSettingInt(
	const UObject* WorldContextObject,
	const TArray<FMCore_IntSettingChange>& Changes,
	bool bBypassConfirmation)
{
	ApplySettingChanges_Internal<FMCore_IntSettingChange, int32>(
		WorldContextObject, Changes, bBypassConfirmation,
		[](const UMCore_DA_SettingDefinition* S, int32 V) {
			return (S->SettingType == EMCore_SettingType::Dropdown && S->DropdownOptions.Num() > 0)
				? FMath::Clamp(V, 0, S->DropdownOptions.Num() - 1) : V;
		},
		[](UMCore_PlayerSettingsSave* Save, const FString& Key, int32 V) { Save->SetIntSetting(Key, V); },
		[](const UMCore_DA_SettingDefinition* S, int32 V) { ApplySettingToEngine(S, 0.f, V, false); });
}

void UMCore_GameSettingsLibrary::SetSettingBool(
	const UObject* WorldContextObject,
	const TArray<FMCore_BoolSettingChange>& Changes,
	bool bBypassConfirmation)
{
	ApplySettingChanges_Internal<FMCore_BoolSettingChange, bool>(
		WorldContextObject, Changes, bBypassConfirmation,
		[](const UMCore_DA_SettingDefinition*, bool V) { return V; },
		[](UMCore_PlayerSettingsSave* Save, const FString& Key, bool V) { Save->SetBoolSetting(Key, V); },
		[](const UMCore_DA_SettingDefinition* S, bool V) { ApplySettingToEngine(S, 0.f, 0, V); });
}

// ============================================================================
// UTILITIES
// ============================================================================

void UMCore_GameSettingsLibrary::ResetSettingToDefault(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting)
{
	if (!Setting) { return; }

	/* const_cast safe: DataAsset from asset registry; const parameter is
	   caller-side guarantee; change struct needs non-const for UPROPERTY serialization */
	UMCore_DA_SettingDefinition* MutableSetting =
		const_cast<UMCore_DA_SettingDefinition*>(Setting);

	switch (Setting->SettingType)
	{
	case EMCore_SettingType::Slider:
		SetSettingFloat(WorldContextObject,
			{ { MutableSetting, Setting->DefaultValue } }, true);
		break;
	case EMCore_SettingType::Toggle:
		SetSettingBool(WorldContextObject,
			{ { MutableSetting, Setting->DefaultToggleValue } }, true);
		break;
	case EMCore_SettingType::Dropdown:
		SetSettingInt(WorldContextObject,
			{ { MutableSetting, Setting->DefaultDropdownIndex } }, true);
		break;
	default:
		break;
	}
}

void UMCore_GameSettingsLibrary::ResetAllSettingsToDefault(const UObject* WorldContextObject)
{
	const TArray<UMCore_DA_SettingsCollection*>& Collections = UMCore_CoreSettings::Get()->GetAllSettingsCollections();
	if (Collections.IsEmpty())
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSettingsLibrary::ResetAllSettingsToDefault -- no settings collections configured in CoreSettings"));
		return;
	}

	TArray<UMCore_DA_SettingDefinition*> Definitions;
	for (const UMCore_DA_SettingsCollection* Collection : Collections)
	{
		for (const TObjectPtr<UMCore_DA_SettingDefinition>& Setting : Collection->GetAllSettings())
		{
			if (Setting) { Definitions.Add(Setting); }
		}
	}

	ResetDefinitionsToDefault(WorldContextObject, Definitions);
}

/**
 * @param CategoryTag  Must match a registered category (e.g., MCore.Settings.Category.Graphics).
 */
void UMCore_GameSettingsLibrary::ResetCategoryToDefault(const UObject* WorldContextObject,
	FGameplayTag CategoryTag)
{
	if (!CategoryTag.IsValid()) { return; }

	ResetDefinitionsToDefault(WorldContextObject,
		UMCore_CoreSettings::Get()->GetSettingsForCategory(CategoryTag));
}

void UMCore_GameSettingsLibrary::ResetDefinitionsToDefault(const UObject* WorldContextObject,
	const TArray<UMCore_DA_SettingDefinition*>& Definitions)
{
	if (Definitions.IsEmpty()) { return; }

	TArray<FMCore_FloatSettingChange> FloatChanges;
	TArray<FMCore_IntSettingChange> IntChanges;
	TArray<FMCore_BoolSettingChange> BoolChanges;

	for (UMCore_DA_SettingDefinition* Setting : Definitions)
	{
		if (!Setting) { continue; }

		switch (Setting->SettingType)
		{
		case EMCore_SettingType::Slider:
			FloatChanges.Add({ Setting, Setting->DefaultValue });
			break;
		case EMCore_SettingType::Toggle:
			BoolChanges.Add({ Setting, Setting->DefaultToggleValue });
			break;
		case EMCore_SettingType::Dropdown:
			IntChanges.Add({ Setting, Setting->DefaultDropdownIndex });
			break;
		default:
			break;
		}
	}

	if (FloatChanges.Num() > 0) { SetSettingFloat(WorldContextObject, FloatChanges, true); }
	if (IntChanges.Num() > 0) { SetSettingInt(WorldContextObject, IntChanges, true); }
	if (BoolChanges.Num() > 0) { SetSettingBool(WorldContextObject, BoolChanges, true); }
}

void UMCore_GameSettingsLibrary::SavePlayerSettings(const UObject* WorldContextObject)
{
	if (UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject))
	{
		Save->SaveSettings();
	}
}

void UMCore_GameSettingsLibrary::ReloadAndApplyFromDisk(const UObject* WorldContextObject)
{
	UMCore_PlayerSettingsSave* CachedSave = GetPlayerSave(WorldContextObject);
	if (!CachedSave)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSettingsLibrary::ReloadAndApplyFromDisk -- no cached PlayerSettingsSave available"));
		return;
	}

	const FString SlotName = CachedSave->GetCachedSlotName();
	if (SlotName.IsEmpty())
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSettingsLibrary::ReloadAndApplyFromDisk -- cached save has no slot name"));
		return;
	}

	UMCore_PlayerSettingsSave* FreshSave = UMCore_PlayerSettingsSave::LoadPlayerSettings(SlotName);
	if (!FreshSave)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSettingsLibrary::ReloadAndApplyFromDisk -- failed to load from slot '%s'"), *SlotName);
		return;
	}

	/* Replace in-memory setting maps with on-disk values */
	CachedSave->FloatSettings = FreshSave->FloatSettings;
	CachedSave->IntSettings = FreshSave->IntSettings;
	CachedSave->BoolSettings = FreshSave->BoolSettings;

	/* Re-apply all settings to engine */
	const UMCore_CoreSettings* CoreSettings = UMCore_CoreSettings::Get();
	if (!CoreSettings)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSettingsLibrary::ReloadAndApplyFromDisk -- CoreSettings unavailable, maps restored but engine not updated"));
		return;
	}

	const TArray<UMCore_DA_SettingsCollection*>& Collections = CoreSettings->GetAllSettingsCollections();
	for (const UMCore_DA_SettingsCollection* Collection : Collections)
	{
		if (!Collection) { continue; }

		for (const TObjectPtr<UMCore_DA_SettingDefinition>& Definition : Collection->GetAllSettings())
		{
			if (!Definition) { continue; }

			const float FloatVal = GetSettingFloat(WorldContextObject, Definition);
			const int32 IntVal = GetSettingInt(WorldContextObject, Definition);
			const bool BoolVal = GetSettingBool(WorldContextObject, Definition);

			ApplySettingToEngine(Definition, FloatVal, IntVal, BoolVal);
		}
	}

	if (UGameUserSettings* GUS = UGameUserSettings::GetGameUserSettings())
	{
		GUS->ApplySettings(false);
	}

	UE_LOG(LogModulusSettings, Log,
		TEXT("GameSettingsLibrary::ReloadAndApplyFromDisk -- restored from slot '%s', %d float / %d int / %d bool settings re-applied"),
		*SlotName, CachedSave->FloatSettings.Num(), CachedSave->IntSettings.Num(), CachedSave->BoolSettings.Num());
}

// ============================================================================
// ENGINE APPLY DISPATCHER
// ============================================================================

void UMCore_GameSettingsLibrary::ApplySettingToEngine(const UMCore_DA_SettingDefinition* Setting, float FloatValue,
	int32 IntValue, bool BoolValue)
{
	if (!Setting) { return; }

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

	if (!Setting->SoundClass.IsNull() && Setting->SettingType == EMCore_SettingType::Slider)
	{
		ApplyToSoundClass(Setting->SoundClass, FloatValue);
	}
}

// ============================================================================
// GAME USER SETTINGS (FPROPERTY REFLECTION)
// ============================================================================

void UMCore_GameSettingsLibrary::ApplyToGameUserSettings(const FName& PropertyName, float Value)
{
	UGameUserSettings* GameSettings = UGameUserSettings::GetGameUserSettings();
	if (!GameSettings) { return; }

	FProperty* ThisProperty = FindFProperty<FProperty>(GameSettings->GetClass(), PropertyName);
	if (!ThisProperty)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSettingsLibrary::ApplyToGameUserSettings -- property '%s' not found"), *PropertyName.ToString());
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
			TEXT("GameSettingsLibrary::ApplyToGameUserSettings -- property '%s' is not float/double"), *PropertyName.ToString());
	}
}

void UMCore_GameSettingsLibrary::ApplyToGameUserSettings(const FName& PropertyName, int32 Value)
{
	UGameUserSettings* GameSettings = UGameUserSettings::GetGameUserSettings();
	if (!GameSettings) { return; }

	FProperty* ThisProperty = FindFProperty<FProperty>(GameSettings->GetClass(), PropertyName);
	if (!ThisProperty)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSettingsLibrary::ApplyToGameUserSettings -- property '%s' not found"), *PropertyName.ToString());
		return;
	}

	if (FIntProperty* IntProp = CastField<FIntProperty>(ThisProperty))
	{
		IntProp->SetPropertyValue_InContainer(GameSettings, Value);
	}
	else
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSettingsLibrary::ApplyToGameUserSettings -- property '%s' is not int32"), *PropertyName.ToString());
	}
}

void UMCore_GameSettingsLibrary::ApplyToGameUserSettings(const FName& PropertyName, bool Value)
{
	UGameUserSettings* GameSettings = UGameUserSettings::GetGameUserSettings();
	if (!GameSettings) { return; }

	FProperty* ThisProperty = FindFProperty<FProperty>(GameSettings->GetClass(), PropertyName);
	if (!ThisProperty)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSettingsLibrary::ApplyToGameUserSettings -- property '%s' not found"), *PropertyName.ToString());
		return;
	}

	if (FBoolProperty* BoolProp = CastField<FBoolProperty>(ThisProperty))
	{
		BoolProp->SetPropertyValue_InContainer(GameSettings, Value);
	}
	else
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSettingsLibrary::ApplyToGameUserSettings -- property '%s' is not bool"), *PropertyName.ToString());
	}
}

// ============================================================================
// CONSOLE VARIABLES
// ============================================================================

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
			TEXT("GameSettingsLibrary::ApplyToConsoleVariable -- console variable '%s' not found"), *CVarName.ToString());
	}
}

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
			TEXT("GameSettingsLibrary::ApplyToConsoleVariable -- console variable '%s' not found"), *CVarName.ToString());
	}
}

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
			TEXT("GameSettingsLibrary::ApplyToConsoleVariable -- console variable '%s' not found"), *CVarName.ToString());
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
			TEXT("GameSettingsLibrary::ApplyToSoundClass -- failed to load SoundClass '%s'"), *SoundClassRef.ToString());
		return;
	}

	LoadedClass->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);

	UE_LOG(LogModulusSettings, Verbose,
		TEXT("GameSettingsLibrary::ApplyToSoundClass -- SoundClass '%s' volume set to %.2f"),
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
