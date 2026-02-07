// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "InputCoreTypes.h"
#include "CoreData/Types/Settings/MCore_SettingsPresets.h"
#include "MCore_GameSettingsLibrary.generated.h"

class UMCore_DA_SettingDefinition;
class UMCore_PlayerSettingsSave;
class USoundClass;

/**
 * Game settings library
 *
 * Orchestrates the DataAsset-driven settings system
 * Also provides thin wrappers around UGameUserSettings for graphics presets.
 */
UCLASS()
class MODULUSCORE_API UMCore_GameSettingsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ========================================================================
	// TYPED GETTERS (pending → committed → DataAsset default)
	// ========================================================================

	/** Get effective float value for a setting (pending → committed → default) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static float GetSettingFloat(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting);

	/** Get effective integer value for a setting */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static int32 GetSettingInt(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting);

	/** Get effective boolean value for a setting */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static bool GetSettingBool(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting);

	/** Get effective key binding for a setting */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static FKey GetSettingKey(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting);

	// ========================================================================
	// TYPED SETTERS (stage to pending)
	// ========================================================================

	/** Stage a float setting change */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void SetSettingFloat(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting, float Value);

	/** Stage an integer setting change */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void SetSettingInt(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting, int32 Value);

	/** Stage a boolean setting change */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void SetSettingBool(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting, bool Value);

	/** Stage a key binding setting change */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void SetSettingKey(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting, const FKey& Value);

	// ========================================================================
	// DEFERRED-APPLY LIFECYCLE
	// ========================================================================

	/**
	 * Apply all pending changes to engine systems and commit to save data.
	 * Equivalent to the user clicking "Apply" in a settings menu.
	 * Commits pending → applies to GUS/CVars/SoundClasses → saves to disk.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void ApplyPendingSettings(const UObject* WorldContextObject,
		const UMCore_DA_SettingsCollection* Collection);

	/**
	 * Discard all pending changes and revert any previewed engine state.
	 * Equivalent to the user clicking "Cancel" in a settings menu.
	 * Re-applies committed values to undo any preview side effects.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void DiscardPendingSettings(const UObject* WorldContextObject,
		const UMCore_DA_SettingsCollection* Collection);

	/**
	 * Preview pending changes by applying to engine systems without committing.
	 * Lets the user hear volume changes / see brightness changes before applying.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void PreviewPendingSettings(const UObject* WorldContextObject,
		const UMCore_DA_SettingsCollection* Collection);

	/** Reset a single setting to its DataAsset default value (staged as pending) */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void ResetSettingToDefault(const UObject* WorldContextObject,
		const UMCore_DA_SettingDefinition* Setting);

	/** Explicitly save player settings to disk */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings",
		meta = (WorldContext = "WorldContextObject"))
	static void SavePlayerSettings(const UObject* WorldContextObject);

	// ========================================================================
	// GRAPHICS PRESETS (thin wrapper around UGameUserSettings)
	// ========================================================================

	/** Apply graphics quality preset (affects all scalability settings) */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings|Graphics")
	static void ApplyGraphicsPreset(EMCore_GraphicsPreset Preset);

	/** Get current graphics preset based on scalability levels */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings|Graphics")
	static EMCore_GraphicsPreset GetCurrentGraphicsPreset();

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

	// ========================================================================
	// GRAPHICS PRESET HELPERS
	// ========================================================================

	static int32 ConvertPresetToScalabilityLevel(EMCore_GraphicsPreset Preset);
	static EMCore_GraphicsPreset ConvertScalabilityLevelToPreset(int32 Level);
};