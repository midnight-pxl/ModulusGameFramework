// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputCoreTypes.h"
#include "GameFramework/SaveGame.h"
#include "MCore_PlayerSettingsSave.generated.h"

/** Delegate called when player settings finish loading asynchronously */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnPlayerSettingsLoaded, UMCore_PlayerSettingsSave*, PlayerSettings);

/**
 * Player settings save game
 *
 * Stores two kinds of data:
 *   1. Framework UI state — UIScale, tooltip delay, collapsed categories, etc.
 *   2. Generic setting values — typed TMap storage keyed by SettingTag save keys,
 *      written by UMCore_GameSettingsLibrary's typed setters.
 *
 * Blueprint Usage:
 *   - Access via UMCore_UISubsystem::GetPlayerSettings()
 *   - Modify via UMCore_GameSettingsLibrary setters (preferred) or direct property access
 *   - SaveSettings() to persist changes
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

    /** UI scale multiplier (0.5 to 3.0) */
    UPROPERTY(SaveGame)
    float UIScale{1.0f};

    /** Tooltip hover delay in milliseconds */
    UPROPERTY(SaveGame)
    int32 TooltipDelayMs{500};

    /** Last settings tab the player had open (restored on re-open) */
    UPROPERTY(SaveGame)
    FGameplayTag LastSelectedCategory;

    /** Which setting categories were collapsed (per tab) */
    UPROPERTY(SaveGame)
    TSet<FGameplayTag> CollapsedCategories;

    /** Has player seen the welcome screen? (prevents showing twice) */
    UPROPERTY(SaveGame)
    bool bHasSeenWelcomeScreen{false};
	
	// ========================================================================
	// GENERIC SETTING STORAGE (DataAsset-driven)
	// ========================================================================

	/** Float values (sliders: volume, sensitivity, brightness, etc.) */
	UPROPERTY(SaveGame)
	TMap<FString, float> FloatSettings;

	/** Integer values (dropdowns: quality presets, resolution index, etc.) */
	UPROPERTY(SaveGame)
	TMap<FString, int32> IntSettings;

	/** Boolean values (toggles: VSync, subtitles, invert-Y, etc.) */
	UPROPERTY(SaveGame)
	TMap<FString, bool> BoolSettings;

	/** Key binding overrides (stored separately from Enhanced Input save) */
	UPROPERTY(SaveGame)
	TMap<FString, FKey> KeySettings;
		
	// ========================================================================
	// PENDING CHANGES (Transient — never serialized to disk)
	// ========================================================================

	/** Sparse maps containing only settings modified since last Apply/Discard */
	UPROPERTY(Transient)
	TMap<FString, float> PendingFloatSettings;

	UPROPERTY(Transient)
	TMap<FString, int32> PendingIntSettings;

	UPROPERTY(Transient)
	TMap<FString, bool> PendingBoolSettings;

	UPROPERTY(Transient)
	TMap<FString, FKey> PendingKeySettings;

	// ========================================================================
	// GENERIC ACCESSORS
	// ========================================================================

	/** Get a float setting. Returns true if the key exists. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	bool GetFloatSetting(const FString& Key, float& OutValue) const;

	/** Set a float setting. */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetFloatSetting(const FString& Key, float Value);

	/** Get an integer setting. Returns true if the key exists. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	bool GetIntSetting(const FString& Key, int32& OutValue) const;

	/** Set an integer setting. */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetIntSetting(const FString& Key, int32 Value);

	/** Get a boolean setting. Returns true if the key exists. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	bool GetBoolSetting(const FString& Key, bool& OutValue) const;

	/** Set a boolean setting. */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetBoolSetting(const FString& Key, bool Value);

	/** Get a key binding setting. Returns true if the key exists. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	bool GetKeySetting(const FString& Key, FKey& OutValue) const;

	/** Set a key binding setting. */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetKeySetting(const FString& Key, const FKey& Value);
	
	// ========================================================================
	// PENDING CHANGE MANAGEMENT
	// ========================================================================
	/** Stage a float change without committing */
	void SetPendingFloat(const FString& Key, float Value);
	/** Stage an integer change without committing */
	void SetPendingInt(const FString& Key, int32 Value);
	/** Stage a boolean change without committing */
	void SetPendingBool(const FString& Key, bool Value);
	/** Stage a key binding change without committing */
	void SetPendingKey(const FString& Key, const FKey& Value);
	
	/** Commit all pending changes to the saved settings */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void CommitPendingSettings();

	/** Discard all pending changes */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void DiscardPendingSettings();

	/** Returns true if any setting has been modified since last Apply/Discard */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	bool HasPendingChanges() const;

	// ========================================================================
	// PERSISTENCE
	// ========================================================================

	/** Save current settings to disk */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SaveSettings();

	/**
	 * Load player settings from disk (synchronous).
	 * Returns existing save if found, or creates new instance with defaults.
	 * Does NOT cache internally — caching is handled by UMCore_UISubsystem.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	static UMCore_PlayerSettingsSave* LoadPlayerSettings();

	/**
	 * Load player settings from disk (asynchronous).
	 * Falls back to synchronous creation if no save file exists.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	static void LoadPlayerSettingsAsync(FOnPlayerSettingsLoaded OnLoaded);

	// ========================================================================
	// FRAMEWORK CONVENIENCE
	// ========================================================================

	/** Set UI scale (clamped 0.5-3.0) and apply immediately */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetUIScale(float NewScale);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	float GetUIScale() const { return UIScale; }

	/** Set tooltip hover delay in milliseconds */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetTooltipDelay(int32 DelayMs);

	/** Mark a setting category as collapsed/expanded in the UI */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetCategoryCollapsed(const FGameplayTag& CategoryTag, bool bCollapsed);

	/** Check if a setting category is collapsed */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	bool IsCategoryCollapsed(const FGameplayTag& CategoryTag) const;

	/** Store which category tab was last active */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void SetLastSelectedCategory(const FGameplayTag& CategoryTag);

	/** Validate all stored values and fix any out-of-range data */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	void ValidateSettings();

private:
	static FString GetSaveSlotName() { return TEXT("ModulusPlayerSettings"); }
	static int32 GetUserIndex() { return 0; }

	/** Apply UIScale to the viewport DPI scaling */
	void ApplyUIScale();
};
