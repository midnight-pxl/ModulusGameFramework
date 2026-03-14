// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreData/Libraries/MCore_GameSettingsLibrary.h"

#include "CoreUI/MCore_UISubsystem.h"
#include "CoreData/Logging/LogModulusSettings.h"
#include "CoreUI/Settings/MCore_PlayerSettingsSave.h"
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
// TYPED GETTERS (committed → DataAsset default)
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
			TEXT("GetSettingInt: called on non-Dropdown setting '%s'"),
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
			TEXT("GetSettingBool: called on non-Toggle setting '%s'"),
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
// TAG-BASED GETTER (resolve via DefaultSettingsCollection)
// ============================================================================

int32 UMCore_GameSettingsLibrary::GetSettingIntByTag(const UObject* WorldContextObject,
	FGameplayTag SettingTag)
{
	const UMCore_DA_SettingsCollection* Collection = UMCore_CoreSettings::Get()->DefaultSettingsCollection;
	if (!Collection)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GetSettingIntByTag: DefaultSettingsCollection is null — cannot resolve tag '%s'"),
			*SettingTag.ToString());
		return 0;
	}

	const UMCore_DA_SettingDefinition* Setting = Collection->FindSettingByTag(SettingTag);
	if (!Setting)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GetSettingIntByTag: No setting found for tag '%s' in DefaultSettingsCollection"),
			*SettingTag.ToString());
		return 0;
	}

	return GetSettingInt(WorldContextObject, Setting);
}

// ============================================================================
// TYPED SETTERS (immediate-apply, batch)
// ============================================================================

void UMCore_GameSettingsLibrary::SetSettingFloat(
	const UObject* WorldContextObject,
	const TArray<FMCore_FloatSettingChange>& Changes,
	bool bBypassConfirmation)
{
	if (!WorldContextObject || Changes.IsEmpty()) { return; }

	UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (!Save)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("SetSettingFloat: Failed to get PlayerSettingsSave"));
		return;
	}

	/** Confirmation tracking across the batch */
	TArray<FString> AffectedTags;
	TArray<FString> PreviousValues;
	TArray<FGameplayTag> ProcessedTags;
	float LongestRevertDelay = 0.f;
	bool bAnyRequiresConfirmation = false;

	/** Pass 1: write all values to committed map and apply to engine */
	for (const FMCore_FloatSettingChange& Change : Changes)
	{
		if (!Change.Setting)
		{
			UE_LOG(LogModulusSettings, Warning,
				TEXT("SetSettingFloat: Null Setting in Changes — skipping"));
			continue;
		}

		const FString Key = Change.Setting->GetSaveKey();
		float ClampedValue = Change.Value;

		/** Clamp to DataAsset-defined range */
		if (Change.Setting->SettingType == EMCore_SettingType::Slider)
		{
			ClampedValue = FMath::Clamp(ClampedValue,
				Change.Setting->MinValue, Change.Setting->MaxValue);
		}

		/** Capture previous committed value before overwriting */
		float PreviousFloat = Change.Setting->DefaultValue;
		Save->GetFloatSetting(Key, PreviousFloat);

		/** Write to committed map */
		Save->SetFloatSetting(Key, ClampedValue);

		/** Apply to engine */
		ApplySettingToEngine(Change.Setting, ClampedValue, 0, false);

		/** Track setting for notification or confirmation */
		if (Change.Setting->bRequiresConfirmation && !bBypassConfirmation)
		{
			bAnyRequiresConfirmation = true;
			AffectedTags.Add(Change.Setting->SettingTag.ToString());
			PreviousValues.Add(FString::SanitizeFloat(PreviousFloat));
			LongestRevertDelay = FMath::Max(
				LongestRevertDelay,
				Change.Setting->ConfirmationRevertDelay);
		}
		else
		{
			ProcessedTags.Add(Change.Setting->SettingTag);
		}
	}

	/** Pass 2: single GUS flush for the entire batch */
	if (UGameUserSettings* GUS = UGameUserSettings::GetGameUserSettings())
	{
		GUS->ApplySettings(false);
	}

	/** Pass 3: save immediately or broadcast confirmation event */
	if (bAnyRequiresConfirmation)
	{
		/** Do not save — UMCore_SettingsRevertCountdown decides
		 *  whether to save (confirm) or revert (discard) */
		TMap<FString, FString> EventParams;
		EventParams.Add(TEXT("SettingTags"),
			FString::Join(AffectedTags, TEXT("|")));
		EventParams.Add(TEXT("PreviousValues"),
			FString::Join(PreviousValues, TEXT("|")));
		EventParams.Add(TEXT("RevertDelay"),
			FString::SanitizeFloat(LongestRevertDelay));

		UMCore_EventFunctionLibrary::BroadcastEvent(
			WorldContextObject,
			MCore_SettingsTags::MCore_Settings_Event_ConfirmationRequired,
			EventParams, EMCore_EventScope::Local);
	}
	else
	{
		SavePlayerSettings(WorldContextObject);
	}

	/** Pass 4: notify listeners of each changed setting (skip reset operations).
	 *  Confirmation-required settings are intentionally excluded — their
	 *  BroadcastSettingChanged is deferred to UMCore_SettingsRevertCountdown,
	 *  which calls it on user confirm using the SettingTags already present
	 *  in the ConfirmationRequired event payload. */
	if (!bBypassConfirmation)
	{
		for (const FGameplayTag& Tag : ProcessedTags)
		{
			BroadcastSettingChanged(WorldContextObject, Tag);
		}
	}
}

void UMCore_GameSettingsLibrary::SetSettingInt(
	const UObject* WorldContextObject,
	const TArray<FMCore_IntSettingChange>& Changes,
	bool bBypassConfirmation)
{
	if (!WorldContextObject || Changes.IsEmpty()) { return; }

	UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (!Save)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("SetSettingInt: Failed to get PlayerSettingsSave"));
		return;
	}

	/** Confirmation tracking across the batch */
	TArray<FString> AffectedTags;
	TArray<FString> PreviousValues;
	TArray<FGameplayTag> ProcessedTags;
	float LongestRevertDelay = 0.f;
	bool bAnyRequiresConfirmation = false;

	/** Pass 1: write all values to committed map and apply to engine */
	for (const FMCore_IntSettingChange& Change : Changes)
	{
		if (!Change.Setting)
		{
			UE_LOG(LogModulusSettings, Warning,
				TEXT("SetSettingInt: Null Setting in Changes — skipping"));
			continue;
		}

		const FString Key = Change.Setting->GetSaveKey();
		int32 ClampedValue = Change.Value;

		/** Clamp to valid dropdown range */
		if (Change.Setting->SettingType == EMCore_SettingType::Dropdown
			&& Change.Setting->DropdownOptions.Num() > 0)
		{
			ClampedValue = FMath::Clamp(ClampedValue,
				0, Change.Setting->DropdownOptions.Num() - 1);
		}

		/** Capture previous committed value before overwriting */
		int32 PreviousInt = Change.Setting->DefaultDropdownIndex;
		Save->GetIntSetting(Key, PreviousInt);

		/** Write to committed map */
		Save->SetIntSetting(Key, ClampedValue);

		/** Apply to engine */
		ApplySettingToEngine(Change.Setting, 0.f, ClampedValue, false);

		/** Track setting for notification or confirmation */
		if (Change.Setting->bRequiresConfirmation && !bBypassConfirmation)
		{
			bAnyRequiresConfirmation = true;
			AffectedTags.Add(Change.Setting->SettingTag.ToString());
			PreviousValues.Add(FString::FromInt(PreviousInt));
			LongestRevertDelay = FMath::Max(
				LongestRevertDelay,
				Change.Setting->ConfirmationRevertDelay);
		}
		else
		{
			ProcessedTags.Add(Change.Setting->SettingTag);
		}
	}

	/** Pass 2: single GUS flush for the entire batch */
	if (UGameUserSettings* GUS = UGameUserSettings::GetGameUserSettings())
	{
		GUS->ApplySettings(false);
	}

	/** Pass 3: save immediately or broadcast confirmation event */
	if (bAnyRequiresConfirmation)
	{
		TMap<FString, FString> EventParams;
		EventParams.Add(TEXT("SettingTags"),
			FString::Join(AffectedTags, TEXT("|")));
		EventParams.Add(TEXT("PreviousValues"),
			FString::Join(PreviousValues, TEXT("|")));
		EventParams.Add(TEXT("RevertDelay"),
			FString::SanitizeFloat(LongestRevertDelay));

		UMCore_EventFunctionLibrary::BroadcastEvent(
			WorldContextObject,
			MCore_SettingsTags::MCore_Settings_Event_ConfirmationRequired,
			EventParams, EMCore_EventScope::Local);
	}
	else
	{
		SavePlayerSettings(WorldContextObject);
	}

	/** Pass 4: notify listeners of each changed setting (skip reset operations).
	 *  Confirmation-required settings are intentionally excluded — their
	 *  BroadcastSettingChanged is deferred to UMCore_SettingsRevertCountdown,
	 *  which calls it on user confirm using the SettingTags already present
	 *  in the ConfirmationRequired event payload. */
	if (!bBypassConfirmation)
	{
		for (const FGameplayTag& Tag : ProcessedTags)
		{
			BroadcastSettingChanged(WorldContextObject, Tag);
		}
	}
}

void UMCore_GameSettingsLibrary::SetSettingBool(
	const UObject* WorldContextObject,
	const TArray<FMCore_BoolSettingChange>& Changes,
	bool bBypassConfirmation)
{
	if (!WorldContextObject || Changes.IsEmpty()) { return; }

	UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);
	if (!Save)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("SetSettingBool: Failed to get PlayerSettingsSave"));
		return;
	}

	/** Confirmation tracking across the batch */
	TArray<FString> AffectedTags;
	TArray<FString> PreviousValues;
	TArray<FGameplayTag> ProcessedTags;
	float LongestRevertDelay = 0.f;
	bool bAnyRequiresConfirmation = false;

	/** Pass 1: write all values to committed map and apply to engine */
	for (const FMCore_BoolSettingChange& Change : Changes)
	{
		if (!Change.Setting)
		{
			UE_LOG(LogModulusSettings, Warning,
				TEXT("SetSettingBool: Null Setting in Changes — skipping"));
			continue;
		}

		const FString Key = Change.Setting->GetSaveKey();

		/** Capture previous committed value before overwriting */
		bool PreviousBool = Change.Setting->DefaultToggleValue;
		Save->GetBoolSetting(Key, PreviousBool);

		/** Write to committed map */
		Save->SetBoolSetting(Key, Change.Value);

		/** Apply to engine */
		ApplySettingToEngine(Change.Setting, 0.f, 0, Change.Value);

		/** Track setting for notification or confirmation */
		if (Change.Setting->bRequiresConfirmation && !bBypassConfirmation)
		{
			bAnyRequiresConfirmation = true;
			AffectedTags.Add(Change.Setting->SettingTag.ToString());
			PreviousValues.Add(PreviousBool ? TEXT("true") : TEXT("false"));
			LongestRevertDelay = FMath::Max(
				LongestRevertDelay,
				Change.Setting->ConfirmationRevertDelay);
		}
		else
		{
			ProcessedTags.Add(Change.Setting->SettingTag);
		}
	}

	/** Pass 2: single GUS flush for the entire batch */
	if (UGameUserSettings* GUS = UGameUserSettings::GetGameUserSettings())
	{
		GUS->ApplySettings(false);
	}

	/** Pass 3: save immediately or broadcast confirmation event */
	if (bAnyRequiresConfirmation)
	{
		TMap<FString, FString> EventParams;
		EventParams.Add(TEXT("SettingTags"),
			FString::Join(AffectedTags, TEXT("|")));
		EventParams.Add(TEXT("PreviousValues"),
			FString::Join(PreviousValues, TEXT("|")));
		EventParams.Add(TEXT("RevertDelay"),
			FString::SanitizeFloat(LongestRevertDelay));

		UMCore_EventFunctionLibrary::BroadcastEvent(
			WorldContextObject,
			MCore_SettingsTags::MCore_Settings_Event_ConfirmationRequired,
			EventParams, EMCore_EventScope::Local);
	}
	else
	{
		SavePlayerSettings(WorldContextObject);
	}

	/** Pass 4: notify listeners of each changed setting (skip reset operations).
	 *  Confirmation-required settings are intentionally excluded — their
	 *  BroadcastSettingChanged is deferred to UMCore_SettingsRevertCountdown,
	 *  which calls it on user confirm using the SettingTags already present
	 *  in the ConfirmationRequired event payload. */
	if (!bBypassConfirmation)
	{
		for (const FGameplayTag& Tag : ProcessedTags)
		{
			BroadcastSettingChanged(WorldContextObject, Tag);
		}
	}
}

// ============================================================================
// UTILITIES
// ============================================================================

void UMCore_GameSettingsLibrary::ResetSettingToDefault(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting)
{
	if (!Setting) { return; }

	/** const_cast is safe: Setting is a UDataAsset from the asset registry.
	 *  The const parameter is a caller-side guarantee; the change struct
	 *  requires a non-const TObjectPtr for UPROPERTY serialization. */
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
	const UMCore_DA_SettingsCollection* Collection = UMCore_CoreSettings::Get()->DefaultSettingsCollection;
	if (!Collection)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("ResetAllSettingsToDefault: DefaultSettingsCollection is null in CoreSettings"));
		return;
	}

	/** Collect non-null definitions from the collection */
	TArray<UMCore_DA_SettingDefinition*> Definitions;
	for (const TObjectPtr<UMCore_DA_SettingDefinition>& Setting : Collection->GetAllSettings())
	{
		if (Setting) { Definitions.Add(Setting); }
	}

	ResetDefinitionsToDefault(WorldContextObject, Definitions);
}

void UMCore_GameSettingsLibrary::ResetCategoryToDefault(const UObject* WorldContextObject,
	FGameplayTag CategoryTag)
{
	if (!CategoryTag.IsValid()) { return; }

	const UMCore_DA_SettingsCollection* Collection = UMCore_CoreSettings::Get()->DefaultSettingsCollection;
	if (!Collection)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("ResetCategoryToDefault: DefaultSettingsCollection is null in CoreSettings"));
		return;
	}

	ResetDefinitionsToDefault(WorldContextObject, Collection->GetSettingsInCategory(CategoryTag));
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
