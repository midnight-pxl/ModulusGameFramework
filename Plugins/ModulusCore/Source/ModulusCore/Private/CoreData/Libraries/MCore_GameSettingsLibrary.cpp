// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreData/Libraries/MCore_GameSettingsLibrary.h"

#include "CoreData/Settings/MCore_PlayerSettingsSubsystem.h"
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
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Types/SlateEnums.h"
#include "Sound/SoundMix.h"
#include "Algo/Reverse.h"

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

	UMCore_PlayerSettingsSubsystem* SettingsSubsystem = LocalPlayer->GetSubsystem<UMCore_PlayerSettingsSubsystem>();
	if (!SettingsSubsystem) { return nullptr; }

	return SettingsSubsystem->GetPlayerSettings();
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
		[WorldContextObject](const UMCore_DA_SettingDefinition* S, float V) { ApplySettingToEngine(WorldContextObject, S, V, 0, false); });
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
		[WorldContextObject](const UMCore_DA_SettingDefinition* S, int32 V) { ApplySettingToEngine(WorldContextObject, S, 0.f, V, false); });
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
		[WorldContextObject](const UMCore_DA_SettingDefinition* S, bool V) { ApplySettingToEngine(WorldContextObject, S, 0.f, 0, V); });
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

			/* Custom intent — skip QualityPreset apply so individual scalability DAs drive engine state.
			   Without this guard, the cascade in ApplyViaGUSSetter would overwrite just-loaded
			   individual save values with engine state matching the saved preset value. */
			static const FName OverallScalabilityProp(TEXT("OverallScalabilityLevel"));
			if (Definition->GameUserSettingsProperty == OverallScalabilityProp
				&& CachedSave->GetLastSelectedQualityPreset() == -1)
			{
				continue;
			}

			const float FloatVal = GetSettingFloat(WorldContextObject, Definition);
			const int32 IntVal = GetSettingInt(WorldContextObject, Definition);
			const bool BoolVal = GetSettingBool(WorldContextObject, Definition);

			ApplySettingToEngine(WorldContextObject, Definition, FloatVal, IntVal, BoolVal);
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

void UMCore_GameSettingsLibrary::ApplySettingToEngine(const UObject* WorldContextObject,
	const UMCore_DA_SettingDefinition* Setting, float FloatValue, int32 IntValue, bool BoolValue)
{
	if (!Setting) { return; }

	/* Phase 1 — GameUserSettings (setter map with FProperty reflection fallback) */
	if (!Setting->GameUserSettingsProperty.IsNone())
	{
		UGameUserSettings* GUS = UGameUserSettings::GetGameUserSettings();
		if (GUS && ApplyViaGUSSetter(Setting->GameUserSettingsProperty, GUS, FloatValue, IntValue, BoolValue, WorldContextObject))
		{
			/* Handled by setter — skip FProperty reflection */
		}
		else
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
	}

	/* Phase 2 — Console Variables */
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

	/* Phase 3 — Sound Class volume (Slider only) */
	if (!Setting->SoundClass.IsNull() && Setting->SettingType == EMCore_SettingType::Slider)
	{
		ApplyToSoundClass(Setting->SoundClass, FloatValue);
	}

	/* Phase 4 — SoundMix push/pop (Toggle only) */
	if (!Setting->PushedSoundMix.IsNull() && Setting->SettingType == EMCore_SettingType::Toggle)
	{
		ApplyToSoundMix(WorldContextObject, Setting->PushedSoundMix,
			Setting->GetSaveKey(), BoolValue);
	}

	/* Phase 5 — Color Vision Deficiency (Slate renderer, client-only) */
	if (Setting->ColorVisionRole != EModulusColorVisionRole::None)
	{
		ApplyToColorVisionDeficiency(Setting, IntValue, FloatValue);
	}
}

// ============================================================================
// GAME USER SETTINGS (SETTER DISPATCH)
// ============================================================================

bool UMCore_GameSettingsLibrary::ApplyViaGUSSetter(const FName& PropertyName, UGameUserSettings* GUS,
	float FloatValue, int32 IntValue, bool BoolValue,
	const UObject* WorldContextObject)
{
	UE_LOG(LogModulusSettings, Verbose,
		TEXT("GameSettingsLibrary::ApplyViaGUSSetter -- dispatch '%s'"), *PropertyName.ToString());

	UMCore_PlayerSettingsSave* Save = GetPlayerSave(WorldContextObject);

	static const FName Name_OverallScalabilityLevel(TEXT("OverallScalabilityLevel"));
	static const FName Name_TextureQuality(TEXT("TextureQuality"));
	static const FName Name_ShadowQuality(TEXT("ShadowQuality"));
	static const FName Name_AntiAliasingQuality(TEXT("AntiAliasingQuality"));
	static const FName Name_PostProcessQuality(TEXT("PostProcessQuality"));
	static const FName Name_ViewDistanceQuality(TEXT("ViewDistanceQuality"));
	static const FName Name_FoliageQuality(TEXT("FoliageQuality"));
	static const FName Name_ShadingQuality(TEXT("ShadingQuality"));
	static const FName Name_EffectsQuality(TEXT("EffectsQuality"));
	static const FName Name_FullscreenMode(TEXT("FullscreenMode"));
	static const FName Name_BUseVSync(TEXT("bUseVSync"));
	static const FName Name_FrameRateLimit(TEXT("FrameRateLimit"));
	static const FName AudioQualityLevelName(TEXT("AudioQualityLevel"));
	static const FName GlobalIlluminationQualityName(TEXT("GlobalIlluminationQuality"));
	static const FName ReflectionQualityName(TEXT("ReflectionQuality"));
	static const FName BUseDynamicResolutionName(TEXT("bUseDynamicResolution"));
	static const FName ResolutionScaleName(TEXT("ResolutionScale"));
	static const FName BUseHDRDisplayOutputName(TEXT("bUseHDRDisplayOutput"));
	static const FName HDRDisplayOutputNitsName(TEXT("HDRDisplayOutputNits"));
	static const FName ScreenResolutionName(TEXT("ScreenResolution"));

	/* Graphics — scalability setters (int32 0–4) */
	if (PropertyName == Name_OverallScalabilityLevel)
	{
		GUS->SetOverallScalabilityLevel(IntValue);

		/* Cascade engine values back to individual save keys so subsequent reloads
		   and per-widget reads reflect what the preset just applied. */
		CascadeScalabilityValuesToSave(Save);
		if (Save) { Save->SetLastSelectedQualityPreset(FMath::Clamp(IntValue, 0, 3)); }

		/* Notify subscribed widgets to refresh from save. */
		UMCore_EventFunctionLibrary::BroadcastSimpleEvent(
			WorldContextObject,
			MCore_SettingsTags::MCore_Settings_Event_ExternalValueChange,
			EMCore_EventScope::Local);
	}
	else if (PropertyName == Name_TextureQuality)
	{
		GUS->ScalabilityQuality.TextureQuality = IntValue;
		MarkQualityPresetCustom(Save);
	}
	else if (PropertyName == Name_ShadowQuality)
	{
		GUS->ScalabilityQuality.ShadowQuality = IntValue;
		MarkQualityPresetCustom(Save);
	}
	else if (PropertyName == Name_AntiAliasingQuality)
	{
		GUS->ScalabilityQuality.AntiAliasingQuality = IntValue;
		MarkQualityPresetCustom(Save);
	}
	else if (PropertyName == Name_PostProcessQuality)
	{
		GUS->ScalabilityQuality.PostProcessQuality = IntValue;
		MarkQualityPresetCustom(Save);
	}
	else if (PropertyName == Name_ViewDistanceQuality)
	{
		GUS->ScalabilityQuality.ViewDistanceQuality = IntValue;
		MarkQualityPresetCustom(Save);
	}
	else if (PropertyName == Name_FoliageQuality)
	{
		GUS->ScalabilityQuality.FoliageQuality = IntValue;
		MarkQualityPresetCustom(Save);
	}
	else if (PropertyName == Name_ShadingQuality)
	{
		GUS->ScalabilityQuality.ShadingQuality = IntValue;
		MarkQualityPresetCustom(Save);
	}
	else if (PropertyName == Name_EffectsQuality)
	{
		GUS->ScalabilityQuality.EffectsQuality = IntValue;
		MarkQualityPresetCustom(Save);
	}
	/* Display — typed setters */
	else if (PropertyName == Name_FullscreenMode)
	{
		GUS->SetFullscreenMode(EWindowMode::ConvertIntToWindowMode(IntValue));
	}
	else if (PropertyName == Name_BUseVSync)
	{
		GUS->SetVSyncEnabled(BoolValue);
	}
	else if (PropertyName == Name_FrameRateLimit)
	{
		GUS->SetFrameRateLimit(FloatValue);
	}
	/* Audio - typed setters */
	else if (PropertyName == AudioQualityLevelName)
	{
		GUS->SetAudioQualityLevel(IntValue);
		return true;
	}
	/* Additional scalability struct members */
	else if (PropertyName == GlobalIlluminationQualityName)
	{
		GUS->ScalabilityQuality.GlobalIlluminationQuality = IntValue;
		MarkQualityPresetCustom(Save);
		return true;
	}
	else if (PropertyName == ReflectionQualityName)
	{
		GUS->ScalabilityQuality.ReflectionQuality = IntValue;
		MarkQualityPresetCustom(Save);
		return true;
	}
	/* Resolution scaling (runtime perf dial, not a scalability group) */
	else if (PropertyName == BUseDynamicResolutionName)
	{
		GUS->SetDynamicResolutionEnabled(BoolValue);
		return true;
	}
	else if (PropertyName == ResolutionScaleName)
	{
		/* SetResolutionScaleValueEx expects 0..100 percentage, not 0..1 normalized */
		GUS->SetResolutionScaleValueEx(FMath::Clamp(FloatValue, 0.0f, 100.0f));
		return true;
	}
	/* HDR — coupled method, requires two entries that both call EnableHDRDisplayOutput */
	else if (PropertyName == BUseHDRDisplayOutputName)
	{
		const int32 CurrentNits = GUS->GetCurrentHDRDisplayNits();
		GUS->EnableHDRDisplayOutput(BoolValue, CurrentNits > 0 ? CurrentNits : 1000);
		return true;
	}
	else if (PropertyName == HDRDisplayOutputNitsName)
	{
		GUS->EnableHDRDisplayOutput(GUS->IsHDREnabled(), IntValue);
		return true;
	}
	/* Resolution — IntValue is the index into the descending-sorted supported resolutions list */
	else if (PropertyName == ScreenResolutionName)
	{
		TArray<FIntPoint> Resolutions;
		UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
		Algo::Reverse(Resolutions);

		if (Resolutions.IsValidIndex(IntValue))
		{
			GUS->SetScreenResolution(Resolutions[IntValue]);
		}
		else
		{
			UE_LOG(LogModulusSettings, Warning,
				TEXT("GameSettingsLibrary::ApplyViaGUSSetter -- resolution index %d out of range (%d available)"),
				IntValue, Resolutions.Num());
		}
		return true;
	}
	else
	{
		return false;
	}

	return true;
}

// ============================================================================
// QUALITY PRESET CASCADE / INTENT
// ============================================================================

void UMCore_GameSettingsLibrary::CascadeScalabilityValuesToSave(UMCore_PlayerSettingsSave* Save)
{
	if (!Save) { return; }

	UGameUserSettings* GUS = UGameUserSettings::GetGameUserSettings();
	if (!GUS) { return; }

	const UMCore_CoreSettings* CoreSettings = UMCore_CoreSettings::Get();
	if (!CoreSettings) { return; }

	static const FName Name_TextureQuality(TEXT("TextureQuality"));
	static const FName Name_ShadowQuality(TEXT("ShadowQuality"));
	static const FName Name_AntiAliasingQuality(TEXT("AntiAliasingQuality"));
	static const FName Name_PostProcessQuality(TEXT("PostProcessQuality"));
	static const FName Name_ViewDistanceQuality(TEXT("ViewDistanceQuality"));
	static const FName Name_FoliageQuality(TEXT("FoliageQuality"));
	static const FName Name_EffectsQuality(TEXT("EffectsQuality"));
	static const FName Name_ShadingQuality(TEXT("ShadingQuality"));
	static const FName Name_GlobalIlluminationQuality(TEXT("GlobalIlluminationQuality"));
	static const FName Name_ReflectionQuality(TEXT("ReflectionQuality"));

	auto ReadMember = [GUS](const FName& Member, int32& OutValue) -> bool
	{
		if      (Member == Name_TextureQuality)            { OutValue = GUS->ScalabilityQuality.TextureQuality;            return true; }
		else if (Member == Name_ShadowQuality)             { OutValue = GUS->ScalabilityQuality.ShadowQuality;             return true; }
		else if (Member == Name_AntiAliasingQuality)       { OutValue = GUS->ScalabilityQuality.AntiAliasingQuality;       return true; }
		else if (Member == Name_PostProcessQuality)        { OutValue = GUS->ScalabilityQuality.PostProcessQuality;        return true; }
		else if (Member == Name_ViewDistanceQuality)       { OutValue = GUS->ScalabilityQuality.ViewDistanceQuality;       return true; }
		else if (Member == Name_FoliageQuality)            { OutValue = GUS->ScalabilityQuality.FoliageQuality;            return true; }
		else if (Member == Name_EffectsQuality)            { OutValue = GUS->ScalabilityQuality.EffectsQuality;            return true; }
		else if (Member == Name_ShadingQuality)            { OutValue = GUS->ScalabilityQuality.ShadingQuality;            return true; }
		else if (Member == Name_GlobalIlluminationQuality) { OutValue = GUS->ScalabilityQuality.GlobalIlluminationQuality; return true; }
		else if (Member == Name_ReflectionQuality)         { OutValue = GUS->ScalabilityQuality.ReflectionQuality;         return true; }
		return false;
	};

	const TArray<UMCore_DA_SettingsCollection*>& Collections = CoreSettings->GetAllSettingsCollections();
	for (const UMCore_DA_SettingsCollection* Collection : Collections)
	{
		if (!Collection) { continue; }
		for (const TObjectPtr<UMCore_DA_SettingDefinition>& Definition : Collection->GetAllSettings())
		{
			if (!Definition) { continue; }
			int32 EngineValue = 0;
			if (ReadMember(Definition->GameUserSettingsProperty, EngineValue))
			{
				Save->SetIntSetting(Definition->GetSaveKey(), EngineValue);
			}
		}
	}
}

void UMCore_GameSettingsLibrary::MarkQualityPresetCustom(UMCore_PlayerSettingsSave* Save)
{
	if (Save) { Save->SetLastSelectedQualityPreset(-1); }
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
// SOUND MIX
// ============================================================================

void UMCore_GameSettingsLibrary::ApplyToSoundMix(const UObject* WorldContextObject,
	TSoftObjectPtr<USoundMix> SoundMixRef, const FString& SaveKey, bool bDesiredActive)
{
	static TMap<FString, bool> PushedState;

	const bool* ExistingState = PushedState.Find(SaveKey);
	if (ExistingState && *ExistingState == bDesiredActive) { return; }

	if (!WorldContextObject) { return; }

	USoundMix* Mix = SoundMixRef.LoadSynchronous();
	if (!Mix)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("GameSettingsLibrary::ApplyToSoundMix -- failed to load SoundMix '%s'"),
			*SoundMixRef.ToString());
		return;
	}

	if (bDesiredActive)
	{
		UGameplayStatics::PushSoundMixModifier(WorldContextObject, Mix);
	}
	else
	{
		UGameplayStatics::PopSoundMixModifier(WorldContextObject, Mix);
	}

	PushedState.Add(SaveKey, bDesiredActive);

	UE_LOG(LogModulusSettings, Verbose,
		TEXT("GameSettingsLibrary::ApplyToSoundMix -- SoundMix '%s' %s (key: %s)"),
		*Mix->GetName(), bDesiredActive ? TEXT("pushed") : TEXT("popped"), *SaveKey);
}

// ============================================================================
// COLOR VISION DEFICIENCY
// ============================================================================

bool UMCore_GameSettingsLibrary::ApplyToColorVisionDeficiency(
	const UMCore_DA_SettingDefinition* Definition, int32 IntValue, float FloatValue)
{
	if (!Definition || Definition->ColorVisionRole == EModulusColorVisionRole::None)
	{
		return false;
	}

	/* One Slate-wide (Type, Severity) slot — bare statics, no save-key keying.
	   ReloadAndApplyFromDisk fires both role DAs at init so both caches converge. */
	static int32 CachedType = 0;       /* 0 == NormalVision */
	static float CachedSeverity = 0.f;

	switch (Definition->ColorVisionRole)
	{
	case EModulusColorVisionRole::DeficiencyType:
		if (Definition->SettingType != EMCore_SettingType::Dropdown)
		{
			UE_LOG(LogModulusSettings, Warning,
				TEXT("GameSettingsLibrary::ApplyToColorVisionDeficiency -- "
					 "DeficiencyType role requires Dropdown SettingType (setting: %s)"),
				*Definition->GetName());
			return false;
		}
		CachedType = FMath::Clamp(IntValue, 0, 3);
		break;

	case EModulusColorVisionRole::DeficiencySeverity:
		if (Definition->SettingType != EMCore_SettingType::Slider)
		{
			UE_LOG(LogModulusSettings, Warning,
				TEXT("GameSettingsLibrary::ApplyToColorVisionDeficiency -- "
					 "DeficiencySeverity role requires Slider SettingType (setting: %s)"),
				*Definition->GetName());
			return false;
		}
		CachedSeverity = FMath::Clamp(FloatValue, 0.f, 10.f);
		break;

	default:
		return false;
	}

	UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(
		static_cast<EColorVisionDeficiency>(CachedType),
		CachedSeverity,
		/*bCorrectDeficiency=*/true,
		/*bShowCorrectionWithDeficiency=*/false);

	UE_LOG(LogModulusSettings, Verbose,
		TEXT("GameSettingsLibrary::ApplyToColorVisionDeficiency -- Type=%d Severity=%.2f"),
		CachedType, CachedSeverity);

	return true;
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
