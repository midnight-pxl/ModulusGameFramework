// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_GameSettingsLibrary.h
 *
 * Blueprint function library orchestrating the DataAsset-driven settings system.
 * Provides typed getters/setters, tag-based lookup, batch operations,
 * and engine-apply logic (GameUserSettings, CVars, SoundClasses).
 */

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
 * Game settings library (immediate-apply model).
 *
 * Each setter writes to committed storage, applies to engine systems,
 * and saves to disk immediately. Settings with bRequiresConfirmation
 * defer the save until the player confirms via a revert countdown.
 *
 * Key Features:
 * - Typed getters with committed-then-default fallback
 * - Batch setters with single GUS flush per call
 * - Confirmation-required flow with event broadcast
 * - Tag-based lookup via DefaultSettingsCollection
 * - Reset helpers (single, category, all)
 *
 * Blueprint Usage:
 *   All public functions are BlueprintCallable. Pass a WorldContextObject
 *   to resolve the local player's save data.
 */
UCLASS()
class MODULUSCORE_API UMCore_GameSettingsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ============================================================================
	// TYPED GETTERS
	// ============================================================================

	/**
	 * Get effective float value for a setting (committed, then DataAsset default).
	 *
	 * @param WorldContextObject  Object used to resolve the local player.
	 * @param Setting             The setting definition to query.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static float GetSettingFloat(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting);

	/**
	 * Get effective integer value for a setting (committed, then DataAsset default).
	 *
	 * @param WorldContextObject  Object used to resolve the local player.
	 * @param Setting             The setting definition to query.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static int32 GetSettingInt(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting);

	/**
	 * Get effective boolean value for a setting (committed, then DataAsset default).
	 *
	 * @param WorldContextObject  Object used to resolve the local player.
	 * @param Setting             The setting definition to query.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static bool GetSettingBool(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting);

	// ============================================================================
	// TAG-BASED GETTER
	// ============================================================================

	/**
	 * Get effective integer value by tag (resolved from CoreSettings::DefaultSettingsCollection).
	 * Use when the caller has a setting tag but not a definition pointer.
	 *
	 * @param WorldContextObject  Object used to resolve the local player.
	 * @param SettingTag          The gameplay tag to look up.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static int32 GetSettingIntByTag(const UObject* WorldContextObject,
		FGameplayTag SettingTag);

	// ============================================================================
	// TYPED SETTERS (IMMEDIATE-APPLY, BATCH)
	// ============================================================================

	/**
	 * Set one or more float settings, apply to engine, and save.
	 * All changes are applied in a single pass with one GUS flush at the end.
	 * If any setting has bRequiresConfirmation and bBypassConfirmation is false,
	 * the values are applied but not saved -- a confirmation event is broadcast instead.
	 *
	 * @param WorldContextObject   Object used to resolve the local player.
	 * @param Changes              Array of setting/value pairs to apply.
	 * @param bBypassConfirmation  Skip confirmation flow (used by reset operations).
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject",
				DisplayName = "Set Setting Float"))
	static void SetSettingFloat(
		const UObject* WorldContextObject,
		const TArray<FMCore_FloatSettingChange>& Changes,
		bool bBypassConfirmation = false);

	/**
	 * Set one or more integer settings, apply to engine, and save.
	 *
	 * @param WorldContextObject   Object used to resolve the local player.
	 * @param Changes              Array of setting/value pairs to apply.
	 * @param bBypassConfirmation  Skip confirmation flow (used by reset operations).
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject",
				DisplayName = "Set Setting Int"))
	static void SetSettingInt(
		const UObject* WorldContextObject,
		const TArray<FMCore_IntSettingChange>& Changes,
		bool bBypassConfirmation = false);

	/**
	 * Set one or more boolean settings, apply to engine, and save.
	 *
	 * @param WorldContextObject   Object used to resolve the local player.
	 * @param Changes              Array of setting/value pairs to apply.
	 * @param bBypassConfirmation  Skip confirmation flow (used by reset operations).
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject",
				DisplayName = "Set Setting Bool"))
	static void SetSettingBool(
		const UObject* WorldContextObject,
		const TArray<FMCore_BoolSettingChange>& Changes,
		bool bBypassConfirmation = false);

	// ============================================================================
	// UTILITIES
	// ============================================================================

	/**
	 * Reset a single setting to its DataAsset default value (applied immediately).
	 *
	 * @param WorldContextObject  Object used to resolve the local player.
	 * @param Setting             The setting definition to reset.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void ResetSettingToDefault(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting);

	/**
	 * Reset all settings in DefaultSettingsCollection to their DataAsset defaults.
	 * Batched with a single GUS flush per type.
	 *
	 * @param WorldContextObject  Object used to resolve the local player.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void ResetAllSettingsToDefault(const UObject* WorldContextObject);

	/**
	 * Reset all settings in a specific category to their DataAsset defaults.
	 * Batched with a single GUS flush per type.
	 *
	 * @param WorldContextObject  Object used to resolve the local player.
	 * @param CategoryTag         The category to reset.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void ResetCategoryToDefault(const UObject* WorldContextObject,
		FGameplayTag CategoryTag);

	/**
	 * Explicitly save player settings to disk.
	 *
	 * @param WorldContextObject  Object used to resolve the local player.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void SavePlayerSettings(const UObject* WorldContextObject);

private:
	// ============================================================================
	// ENGINE APPLY HELPERS
	// ============================================================================

	/**
	 * Apply a single setting value to its configured engine targets.
	 * Reads GameUserSettingsProperty, ConsoleVariable, and SoundClass
	 * from the DataAsset to determine where to write.
	 */
	static void ApplySettingToEngine(const UMCore_DA_SettingDefinition* Setting,
		float FloatValue, int32 IntValue, bool BoolValue);

	static void ApplyToGameUserSettings(const FName& PropertyName, float Value);
	static void ApplyToGameUserSettings(const FName& PropertyName, int32 Value);
	static void ApplyToGameUserSettings(const FName& PropertyName, bool Value);

	static void ApplyToConsoleVariable(const FName& CVarName, float Value);
	static void ApplyToConsoleVariable(const FName& CVarName, int32 Value);
	static void ApplyToConsoleVariable(const FName& CVarName, bool Value);

	static void ApplyToSoundClass(const TSoftObjectPtr<USoundClass>& SoundClassRef, float Volume);

	static void BroadcastSettingChanged(const UObject* WorldContextObject,
		const FGameplayTag& SettingTag);

	static UMCore_PlayerSettingsSave* GetPlayerSave(const UObject* WorldContextObject);

	static void ResetDefinitionsToDefault(const UObject* WorldContextObject,
		const TArray<UMCore_DA_SettingDefinition*>& Definitions);
};
