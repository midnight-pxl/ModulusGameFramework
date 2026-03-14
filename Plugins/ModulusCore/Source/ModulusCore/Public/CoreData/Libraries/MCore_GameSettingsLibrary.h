// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "CoreData/Types/Settings/MCore_SettingsTypes.h"
#include "MCore_GameSettingsLibrary.generated.h"

class UMCore_DA_SettingDefinition;
class UMCore_PlayerSettingsSave;
class USoundClass;

/**
 * Game settings library (immediate-apply model)
 *
 * Orchestrates the DataAsset-driven settings system.
 * Each setter writes to committed storage, applies to engine systems,
 * and saves to disk immediately. Settings with bRequiresConfirmation
 * defer the save until the player confirms via a revert countdown.
 */
UCLASS()
class MODULUSCORE_API UMCore_GameSettingsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ========================================================================
	// TYPED GETTERS (committed → DataAsset default)
	// ========================================================================

	/** Get effective float value for a setting (committed → default) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static float GetSettingFloat(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting);

	/** Get effective integer value for a setting (committed → default) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static int32 GetSettingInt(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting);

	/** Get effective boolean value for a setting (committed → default) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static bool GetSettingBool(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting);

	// ========================================================================
	// TAG-BASED GETTER (resolve via DefaultSettingsCollection)
	// ========================================================================

	/**
	 * Get effective integer value by tag (resolved from CoreSettings::DefaultSettingsCollection).
	 * Use when the caller has a setting tag but not a definition pointer
	 * (e.g. core subsystems reading a single known setting).
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static int32 GetSettingIntByTag(const UObject* WorldContextObject,
		FGameplayTag SettingTag);

	// ========================================================================
	// TYPED SETTERS (immediate-apply, batch)
	// ========================================================================

	/**
	 * Set one or more float settings, apply to engine, and save.
	 * All changes are applied in a single pass with one GUS flush at the end.
	 * If any setting has bRequiresConfirmation and bBypassConfirmation is false,
	 * the values are applied but not saved — a confirmation event is broadcast instead.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject",
				DisplayName = "Set Setting Float"))
	static void SetSettingFloat(
		const UObject* WorldContextObject,
		const TArray<FMCore_FloatSettingChange>& Changes,
		bool bBypassConfirmation = false);

	/** Set one or more integer settings, apply to engine, and save. */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject",
				DisplayName = "Set Setting Int"))
	static void SetSettingInt(
		const UObject* WorldContextObject,
		const TArray<FMCore_IntSettingChange>& Changes,
		bool bBypassConfirmation = false);

	/** Set one or more boolean settings, apply to engine, and save. */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject",
				DisplayName = "Set Setting Bool"))
	static void SetSettingBool(
		const UObject* WorldContextObject,
		const TArray<FMCore_BoolSettingChange>& Changes,
		bool bBypassConfirmation = false);

	// ========================================================================
	// UTILITIES
	// ========================================================================

	/** Reset a single setting to its DataAsset default value (applied immediately) */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void ResetSettingToDefault(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting);

	/** Reset all settings in DefaultSettingsCollection to their DataAsset defaults (batched, single GUS flush per type) */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void ResetAllSettingsToDefault(const UObject* WorldContextObject);

	/** Reset all settings in a specific category to their DataAsset defaults (batched, single GUS flush per type) */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void ResetCategoryToDefault(const UObject* WorldContextObject,
		FGameplayTag CategoryTag);

	/** Explicitly save player settings to disk */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void SavePlayerSettings(const UObject* WorldContextObject);

private:
	// ========================================================================
	// ENGINE APPLY HELPERS
	// ========================================================================

	/**
	 * Apply a single setting value to its configured engine targets.
	 * Reads the DataAsset's GameUserSettingsProperty, ConsoleVariable, and SoundClass
	 * to determine where to write.
	 */
	static void ApplySettingToEngine(const UMCore_DA_SettingDefinition* Setting,
		float FloatValue, int32 IntValue, bool BoolValue);

	/** Write a float to a UGameUserSettings property via FProperty reflection */
	static void ApplyToGameUserSettings(const FName& PropertyName, float Value);

	/** Write an int32 to a UGameUserSettings property via FProperty reflection */
	static void ApplyToGameUserSettings(const FName& PropertyName, int32 Value);

	/** Write a bool to a UGameUserSettings property via FProperty reflection */
	static void ApplyToGameUserSettings(const FName& PropertyName, bool Value);

	/** Write a float value to a console variable */
	static void ApplyToConsoleVariable(const FName& CVarName, float Value);

	/** Write an int32 value to a console variable */
	static void ApplyToConsoleVariable(const FName& CVarName, int32 Value);

	/** Write a bool value to a console variable */
	static void ApplyToConsoleVariable(const FName& CVarName, bool Value);

	/** Set volume on a SoundClass via soft reference */
	static void ApplyToSoundClass(const TSoftObjectPtr<USoundClass>& SoundClassRef, float Volume);

	/** Broadcast a setting changed event via the ModulusCore event system */
	static void BroadcastSettingChanged(const UObject* WorldContextObject,
		const FGameplayTag& SettingTag);

	/** Get PlayerSettingsSave from UISubsystem via world context */
	static UMCore_PlayerSettingsSave* GetPlayerSave(const UObject* WorldContextObject);

	/** Batch-reset a list of definitions to their DataAsset defaults via the typed setters */
	static void ResetDefinitionsToDefault(const UObject* WorldContextObject,
		const TArray<UMCore_DA_SettingDefinition*>& Definitions);
};