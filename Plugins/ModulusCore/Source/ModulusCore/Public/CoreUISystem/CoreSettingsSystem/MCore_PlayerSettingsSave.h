// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreData/CoreStructEnums/SettingsStructsEnums/MCore_SettingsPresets.h"
#include "GameFramework/SaveGame.h"
#include "MCore_PlayerSettingsSave.generated.h"

/** Delegate called when player settings finish loading asynchronously */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnPlayerSettingsLoaded, UMCore_PlayerSettingsSave*, PlayerSettings);

/**
 * Player settings save game
 *
 * Stores UI preferences and menu state (not gameplay settings - use UGameUserSettings for those).
 * Automatically saved when modified.
 *
 * Saved Data:
 * - UI Scale
 * - Tooltip delay
 * - Last selected settings tab
 * - Collapsed category states
 * - First-time experience flags
 *
 * Blueprint Usage:
 * - LoadPlayerSettings() to get saved instance
 * - Modify properties
 * - SaveSettings() to persist changes
 *
 * Note: Implementations are currently stubs - awaiting completion.
 */
UCLASS()
class MODULUSCORE_API UMCore_PlayerSettingsSave : public USaveGame
{
	GENERATED_BODY()

	public:
    UMCore_PlayerSettingsSave();

    /** UI scale multiplier (0.5 to 2.0) */
    UPROPERTY(SaveGame)
    float UIScale{1.0f};

    /** Tooltip hover delay in milliseconds */
    UPROPERTY(SaveGame)
    int32 TooltipDelayMs{500};

    /** Last settings tab the player had open (restored on re-open) */
    UPROPERTY(SaveGame)
    EMCore_SettingsTab LastSelectedTab = EMCore_SettingsTab::Graphics;

    /** Which setting categories were collapsed (per tab) */
    UPROPERTY(SaveGame)
    TMap<EMCore_SettingsTab, bool> CollapsedCategories;

    /** Has player seen the welcome screen? (prevents showing twice) */
    UPROPERTY(SaveGame)
    bool bHasSeenWelcomeScreen = false;

    /** Save current settings to disk. Call after modifying properties */
    UFUNCTION(BlueprintCallable, Category="Player Settings")
    void SaveSettings();

    /**
     * Load player settings from disk (synchronous)
     *
     * Returns existing save if found, or creates new instance with defaults.
     *
     * @return Player settings instance
     */
    UFUNCTION(BlueprintCallable, Category="Player Settings")
    static UMCore_PlayerSettingsSave* LoadPlayerSettings();

    /**
     * Load player settings from disk (asynchronous)
     *
     * Useful for loading screens. OnLoaded callback receives settings instance.
     *
     * @param OnLoaded - Delegate called when loading completes
     */
    UFUNCTION(BlueprintCallable, Category="Player Settings")
    static void LoadPlayerSettingsAsync(FOnPlayerSettingsLoaded OnLoaded);

    /** Convenience Functions */
    UFUNCTION(BlueprintCallable, Category="Player Settings")
    void SetUIScale(float NewScale);
    
    UFUNCTION(BlueprintPure, Category="Player Settings")
    float GetUIScale() const { return UIScale; }
    
    UFUNCTION(BlueprintCallable, Category="Player Settings")
    void SetTooltipDelay(int32 DelayMs);
    
    UFUNCTION(BlueprintCallable, Category="Player Settings")
    void MarkCategoryCollapsed(const FString& CategoryName, bool bCollapsed);
    
    UFUNCTION(BlueprintPure, Category="Player Settings")
    bool IsCategoryCollapsed(const FString& CategoryName) const;
    
    UFUNCTION(BlueprintCallable, Category="Player Settings")
	void SetLastSelectedTab(EMCore_SettingsTab Tab) { LastSelectedTab = Tab; SaveSettings(); }

private:
    static FString GetSaveSlotName() { return TEXT("ModulusPlayerSettings"); }
    static int32 GetUserIndex() { return 0; }

    /** Cached instance for performance */
    static TWeakObjectPtr<UMCore_PlayerSettingsSave> CachedSettings;
    
    void ApplyUIScale();
    void ValidateSettings();
};
