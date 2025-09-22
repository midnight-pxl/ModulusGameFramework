// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreData/CoreStructEnums/UIStructsEnums/MCore_UITypes.h"
#include "GameFramework/SaveGame.h"
#include "MCore_PlayerSettingsSave.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnPlayerSettingsLoaded, UMCore_PlayerSettingsSave*, PlayerSettings);

/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_PlayerSettingsSave : public USaveGame
{
	GENERATED_BODY()

	public:
    UMCore_PlayerSettingsSave();

    // Core UI Preferences (high value to remember)
    UPROPERTY(SaveGame)
    float UIScale{1.0f};
    
    UPROPERTY(SaveGame)
    int32 TooltipDelayMs{500};

    // Menu State (improves UX by remembering user choices)
    UPROPERTY(SaveGame)
    EMCore_SettingsTab LastSelectedTab = EMCore_SettingsTab::Graphics;
    
    UPROPERTY(SaveGame)
    TMap<EMCore_SettingsTab, bool> CollapsedCategories;
    
    // First-time experience
    UPROPERTY(SaveGame)
    bool bHasSeenWelcomeScreen = false;

    // Save/Load Interface
    UFUNCTION(BlueprintCallable, Category="Player Settings")
    void SaveSettings();
    
    UFUNCTION(BlueprintCallable, Category="Player Settings")
    static UMCore_PlayerSettingsSave* LoadPlayerSettings();
    
    UFUNCTION(BlueprintCallable, Category="Player Settings")
    static void LoadPlayerSettingsAsync(FOnPlayerSettingsLoaded OnLoaded);

    // Convenience Functions
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
    
    // Cached instance for performance
    static TWeakObjectPtr<UMCore_PlayerSettingsSave> CachedSettings;
    
    void ApplyUIScale();
    void ValidateSettings();
};
