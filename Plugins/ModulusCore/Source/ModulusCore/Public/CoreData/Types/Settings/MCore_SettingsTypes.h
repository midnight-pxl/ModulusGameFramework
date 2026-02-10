// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MCore_SettingsTypes.generated.h"

class UInputAction;

/** Setting widget types for game settings menus */
UENUM(BlueprintType)
enum class EMCore_SettingType : uint8
{
    Toggle       UMETA(DisplayName = "Toggle (Boolean)"),
    Slider       UMETA(DisplayName = "Slider (Float)"),
    Dropdown     UMETA(DisplayName = "Dropdown (Selection)"),
    KeyBinding   UMETA(DisplayName = "Key Binding")
};

/**
 * Setting definition struct
 *
 * Describes a single game setting (display name, type, value range, defaults).
 * Used by UMCore_GameSettingTemplates to create setting categories.
 *
 * Usage:
 * - Create manually in Blueprint/C++, or
 * - Use UMCore_GameSettingTemplates factory functions
 *
 * Properties are type-conditional (e.g., MinValue only visible for Sliders).
 */
USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_DiscoveredKeyBinding
{
    GENERATED_BODY()

    /** Display name from UPlayerMappableKeySettings */
    UPROPERTY(BlueprintReadOnly, Category = "ModulusCore|Input")
    FText DisplayName;

    /** Category from UPlayerMappableKeySettings::DisplayCategory */
    UPROPERTY(BlueprintReadOnly, Category = "ModulusCore|Input")
    FText Description;

    /** Action FName for profile lookups */
    UPROPERTY(BlueprintReadOnly, Category = "ModulusCore|Input")
    FName ActionName;

    /** Player's current key for this action */
    UPROPERTY(BlueprintReadOnly, Category = "ModulusCore|Input")
    FKey CurrentKey;

    /** Source InputAction asset reference */
    UPROPERTY(BlueprintReadOnly, Category = "ModulusCore|Input")
    TObjectPtr<const UInputAction> InputAction = nullptr;
};

/**
 * Group of discovered key bindings sharing a display category.
 * Returned by GetAllKeyBindingCategories for organized rebind UI.
 */
USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_KeyBindingGroup
{
    GENERATED_BODY()
    
    /** Display name for this category (e.g., "Movement", "Combat", "Inventory") */
    UPROPERTY(BlueprintReadOnly, Category = "ModulusCore|Input")
    FText CategoryName;
    
    /** All key binding settings in this category */
    UPROPERTY(BlueprintReadOnly, Category = "ModulusCore|Input")
    TArray<FMCore_DiscoveredKeyBinding> KeyBindings;
};