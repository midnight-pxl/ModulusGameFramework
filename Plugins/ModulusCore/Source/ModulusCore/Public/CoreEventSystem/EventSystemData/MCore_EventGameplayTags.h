// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MCore_EventGameplayTags.generated.h"

// Forward declaration
enum class EMCore_EventScope : uint8;

/**
 * Event tag naming conventions:
 * 
 * MCore.Events.UI.*        - Use Local scope (UI updates)
 * MCore.Events.Player.*    - Use Global scope (player state)  
 * MCore.Events.Gameplay.*  - Use Global scope (game state)
 * MCore.Events.Local.*     - Always Local scope (effects)
 * 
 * This is guidance, not enforcement. You can use any tags you want.
 */

/**
 * Example Event Tags (Use these or create your own hierarchy)
 */ 

namespace MCoreEventTags
{
    // Common UI events (Local scope recommended)
    MODULUSCORE_API extern const FGameplayTag UI_MenuOpened;     // MCore.Events.UI.MenuOpened
    MODULUSCORE_API extern const FGameplayTag UI_MenuClosed;     // MCore.Events.UI.MenuClosed
    MODULUSCORE_API extern const FGameplayTag UI_SettingsChanged; // MCore.Events.UI.SettingsChanged
    
    // Common Player events (Global scope recommended)
    MODULUSCORE_API extern const FGameplayTag Player_LevelUp;    // MCore.Events.Player.LevelUp
    MODULUSCORE_API extern const FGameplayTag Player_ItemFound;  // MCore.Events.Player.ItemFound
    MODULUSCORE_API extern const FGameplayTag Player_QuestComplete; // MCore.Events.Player.QuestComplete
    
    // Common Gameplay events (Global scope recommended)
    MODULUSCORE_API extern const FGameplayTag Gameplay_EnemyKilled; // MCore.Events.Gameplay.EnemyKilled
    MODULUSCORE_API extern const FGameplayTag Gameplay_AreaEntered; // MCore.Events.Gameplay.AreaEntered
    MODULUSCORE_API extern const FGameplayTag Gameplay_ObjectiveComplete; // MCore.Events.Gameplay.ObjectiveComplete
    
    // Local effects (Always Local scope)
    MODULUSCORE_API extern const FGameplayTag Local_SoundPlay;   // MCore.Events.Local.SoundPlay
    MODULUSCORE_API extern const FGameplayTag Local_EffectSpawn; // MCore.Events.Local.EffectSpawn
    MODULUSCORE_API extern const FGameplayTag Local_CameraShake; // MCore.Events.Local.CameraShake
}

// =================================================================
// SIMPLE TAG UTILITIES
// =================================================================

UCLASS()
class MODULUSCORE_API UMCore_GameplayTagUtilities : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Suggest scope based on tag naming convention.
     * Returns Local for UI/Local tags, Global for Player/Gameplay tags.
     */
    UFUNCTION(BlueprintPure, Category = "Modulus Events", CallInEditor)
    static EMCore_EventScope GetSuggestedScope(const FGameplayTag& EventTag);

    /**
     * Check if tag follows ModulusCore naming conventions.
     */
    UFUNCTION(BlueprintPure, Category = "Modulus Events", CallInEditor)
    static bool IsValidEventTag(const FGameplayTag& EventTag);

    /**
     * Get all common event tags for reference.
     */
    UFUNCTION(BlueprintPure, Category = "Modulus Events", CallInEditor)
    static TArray<FGameplayTag> GetCommonEventTags();

#if WITH_EDITOR
    /**
     * Simple validation report for editor use.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Events", CallInEditor)
    static FString GenerateTagUsageReport();
#endif
};