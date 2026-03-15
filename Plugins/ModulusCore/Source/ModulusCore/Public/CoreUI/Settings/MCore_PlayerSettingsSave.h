// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_PlayerSettingsSave.h
 *
 * Save game storing framework UI state and generic typed setting values.
 * Immediate-apply model: changes write to committed storage directly.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "MCore_PlayerSettingsSave.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnPlayerSettingsLoaded, UMCore_PlayerSettingsSave*, PlayerSettings);

/**
 * Player settings save game (immediate-apply model).
 *
 * Stores two kinds of data:
 *   1. Framework UI state -- UIScale, tooltip delay, collapsed categories, etc.
 *   2. Generic setting values -- typed TMap storage keyed by SettingTag save keys,
 *      written by UMCore_GameSettingsLibrary's typed setters.
 *
 * Key Features:
 * - Typed TMap storage (float, int32, bool) for DataAsset-driven settings
 * - Framework UI state persistence (scale, tooltip delay, category collapse)
 * - Sync and async load with automatic validation
 *
 * Blueprint Usage:
 *   Access via UMCore_UISubsystem::GetPlayerSettings().
 *   Modify via UMCore_GameSettingsLibrary setters (preferred) or direct property access.
 *   Call SaveSettings() to persist changes.
 */
UCLASS()
class MODULUSCORE_API UMCore_PlayerSettingsSave : public USaveGame
{
	GENERATED_BODY()

	public:
    UMCore_PlayerSettingsSave();

	// ========================================================================
	// FRAMEWORK UI STATE
	// ========================================================================

    /* UI scale multiplier (0.5 to 3.0) */
    UPROPERTY(SaveGame)
    float UIScale{1.0f};

    /* Tooltip hover delay in milliseconds */
    UPROPERTY(SaveGame)
    int32 TooltipDelayMs{500};

    /* Last settings tab the player had open (restored on re-open) */
    UPROPERTY(SaveGame)
    FGameplayTag LastSelectedCategory;

    UPROPERTY(SaveGame)
    TSet<FGameplayTag> CollapsedCategories;

    UPROPERTY(SaveGame)
    bool bHasSeenWelcomeScreen{false};

	// ========================================================================
	// GENERIC SETTING STORAGE
	// ========================================================================

	UPROPERTY(SaveGame)
	TMap<FString, float> FloatSettings;

	UPROPERTY(SaveGame)
	TMap<FString, int32> IntSettings;

	UPROPERTY(SaveGame)
	TMap<FString, bool> BoolSettings;

	// ========================================================================
	// GENERIC ACCESSORS
	// ========================================================================

	/**
	 * Get a float setting value.
	 *
	 * @param Key       Save key from the setting definition.
	 * @param OutValue  Populated if key exists.
	 * @return True if the key was found.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	bool GetFloatSetting(const FString& Key, float& OutValue) const;

	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetFloatSetting(const FString& Key, float Value);

	/**
	 * Get an integer setting value.
	 *
	 * @param Key       Save key from the setting definition.
	 * @param OutValue  Populated if key exists.
	 * @return True if the key was found.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	bool GetIntSetting(const FString& Key, int32& OutValue) const;

	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetIntSetting(const FString& Key, int32 Value);

	/**
	 * Get a boolean setting value.
	 *
	 * @param Key       Save key from the setting definition.
	 * @param OutValue  Populated if key exists.
	 * @return True if the key was found.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	bool GetBoolSetting(const FString& Key, bool& OutValue) const;

	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetBoolSetting(const FString& Key, bool Value);

	// ========================================================================
	// PERSISTENCE
	// ========================================================================

	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SaveSettings();

	/**
	 * Load player settings from disk (synchronous).
	 * Returns existing save if found, or creates new instance with defaults.
	 * Does NOT cache internally -- caching is handled by UMCore_UISubsystem.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	static UMCore_PlayerSettingsSave* LoadPlayerSettings();

	/**
	 * Load player settings from disk (asynchronous).
	 * Falls back to synchronous creation if no save file exists.
	 *
	 * @param OnLoaded  Callback fired when loading completes.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	static void LoadPlayerSettingsAsync(FOnPlayerSettingsLoaded OnLoaded);

	// ========================================================================
	// FRAMEWORK CONVENIENCE
	// ========================================================================

	/**
	 * Set UI scale (clamped 0.5-3.0) and apply immediately.
	 *
	 * @param NewScale  Desired UI scale multiplier.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetUIScale(float NewScale);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	float GetUIScale() const { return UIScale; }

	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetTooltipDelay(int32 DelayMs);

	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetCategoryCollapsed(const FGameplayTag& CategoryTag, bool bCollapsed);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	bool IsCategoryCollapsed(const FGameplayTag& CategoryTag) const;

	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetLastSelectedCategory(const FGameplayTag& CategoryTag);

	/**
	 * Validate all stored values and fix any out-of-range data.
	 *
	 * Blueprint Usage:
	 *   Called automatically after load. Can be called manually after bulk edits.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void ValidateSettings();

private:
	static FString GetSaveSlotName() { return TEXT("ModulusPlayerSettings"); }
	static int32 GetUserIndex() { return 0; }

	void ApplyUIScale();
};
