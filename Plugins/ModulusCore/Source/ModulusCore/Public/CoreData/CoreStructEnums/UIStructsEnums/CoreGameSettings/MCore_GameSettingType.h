// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MCore_GameSettingType.generated.h"

class UInputMappingContext;
class UInputAction;
class UCommonActivatableWidget;

/** Setting widget types for game settings menus */
UENUM(BlueprintType)
enum class EMCore_SettingType : uint8
{
    Toggle       UMETA(DisplayName = "Toggle (Boolean)"),
    Slider       UMETA(DisplayName = "Slider (Float)"),
    Dropdown     UMETA(DisplayName = "Dropdown (Selection)"),
    TextInput    UMETA(DisplayName = "Text Input"),
    KeyBinding   UMETA(DisplayName = "Key Binding"),
    Action       UMETA(DisplayName = "Action Button")
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
struct MODULUSCORE_API FMCore_SettingDefinition
{
    GENERATED_BODY()

    // Basic Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
    EMCore_SettingType SettingType = EMCore_SettingType::Toggle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic", 
    meta = (Categories = "Settings"))  // Filters to Settings.* tags only
    FGameplayTag SettingTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeyBinding",
    meta = (EditCondition = "SettingType == EMCore_SettingType::KeyBinding", EditConditionHides))
    TObjectPtr<const UInputAction> InputAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeyBinding",
        meta = (EditCondition = "SettingType == EMCore_SettingType::KeyBinding", EditConditionHides))
    TObjectPtr<const UInputMappingContext> InputMappingContext;

    // Type-Specific Properties (using EditCondition for clean UI)
    
    // For Sliders
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider", 
        meta = (EditCondition = "SettingType == EMCore_SettingType::Slider", ClampMin = "0.0"))
    float MinValue{0.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider",
        meta = (EditCondition = "SettingType == EMCore_SettingType::Slider", ClampMin = "0.0"))
    float MaxValue{1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider",
        meta = (EditCondition = "SettingType == EMCore_SettingType::Slider"))
    float DefaultValue{0.5f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider",
        meta = (EditCondition = "SettingType == EMCore_SettingType::Slider"))
    float StepSize{0.02f};

    // For Toggles
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toggle",
        meta = (EditCondition = "SettingType == EMCore_SettingType::Toggle"))
    bool DefaultToggleValue{false};

    // For Dropdowns
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dropdown",
        meta = (EditCondition = "SettingType == EMCore_SettingType::Dropdown"))
    TArray<FText> DropdownOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dropdown",
        meta = (EditCondition = "SettingType == EMCore_SettingType::Dropdown", ClampMin = "0"))
    int32 DefaultDropdownIndex{0};

    // For Text Input
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text",
        meta = (EditCondition = "SettingType == EMCore_SettingType::TextInput"))
    FString DefaultTextValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text",
        meta = (EditCondition = "SettingType == EMCore_SettingType::TextInput", ClampMin = "1"))
    int32 MaxTextLength{100};

    // For Key Bindings  
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key Binding",
        meta = (EditCondition = "SettingType == EMCore_SettingType::KeyBinding"))
    FKey DefaultKey;

    // For Actions (buttons that trigger functions)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action",
        meta = (EditCondition = "SettingType == EMCore_SettingType::Action"))
    FText ActionButtonText = FText::FromString("Execute");

    // Helper function to validate a setting
    bool IsValid() const
    {
        if (DisplayName.IsEmpty() || !SettingTag.IsValid()) { return false; }

        switch (SettingType)
        {
        case EMCore_SettingType::Slider:
            return ((MinValue <= MaxValue) && (DefaultValue >= MinValue) && (DefaultValue <= MaxValue));
        case EMCore_SettingType::Dropdown:
            return ((DropdownOptions.Num() > 0) && (DefaultDropdownIndex >= 0) && (DefaultDropdownIndex < DropdownOptions.Num()));
        default:
            return true;
        }
    }

    FString GetSettingKeyString() const
    {
        return SettingTag.IsValid() ? SettingTag.ToString() : FString();
    }
};

/**
 * Settings category struct
 *
 * Groups related settings together (e.g., Graphics, Audio, Controls).
 * Contains array of FMCore_SettingDefinition with shared category metadata.
 *
 * Example:
 * - CategoryName: "Graphics"
 * - Settings: [Quality Preset, Texture Quality, Shadow Quality, Anti-Aliasing]
 */
USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_SettingCategory
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Category")
    FText CategoryName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Category")
    FText CategoryDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Category", meta = (Categories = "Settings.Category"))
    FGameplayTag CategoryTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Category")
    TArray<FMCore_SettingDefinition> Settings;

    // Validation
    bool IsValid() const
    {
        if (CategoryName.IsEmpty() || Settings.IsEmpty())
        {
            return false;
        }

        // Check all settings are valid
        for (const FMCore_SettingDefinition& Setting : Settings)
        {
            if (!Setting.IsValid())
            {
                return false;
            }
        }

        return true;
    }

    // Helper to find setting by save key
    const FMCore_SettingDefinition* FindSetting(const FGameplayTag& SettingTag) const
    {
        return Settings.FindByPredicate([&SettingTag](const FMCore_SettingDefinition& Setting)
        {
            return (Setting.SettingTag == SettingTag);
        });
    }
};

/**
 * Complete settings configuration
 *
 * Top-level container for all setting categories.
 * Pass this to your settings menu to generate the entire UI.
 *
 * Usage:
 * 1. Create via UMCore_GameSettingTemplates factory functions
 * 2. Add/remove categories as needed
 * 3. Pass to settings menu widget
 */
USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_SettingsConfiguration
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    TArray<FMCore_SettingCategory> Categories;

    UPROPERTY()
    FGuid ConfigurationID;

    FMCore_SettingsConfiguration() { ConfigurationID = FGuid::NewGuid(); }

    // Validation
    bool IsValid() const
    {
        if (Categories.IsEmpty())
        {
            return false;
        }

        for (const FMCore_SettingCategory& Category : Categories)
        {
            if (!Category.IsValid())
            {
                return false;
            }
        }

        return true;
    }

    // Helper to find category by name
    const FMCore_SettingCategory* FindCategory(const FGameplayTag& CategoryTag) const
    {
        return Categories.FindByPredicate([&CategoryTag](const FMCore_SettingCategory& Category)
        {
            return Category.CategoryTag == CategoryTag;
        });
    }

    // Helper to find setting across all categories
    const FMCore_SettingDefinition* FindSetting(const FGameplayTag& SettingTag) const
    {
        for (const FMCore_SettingCategory& Category : Categories)
        {
            if (const FMCore_SettingDefinition* Found = Category.FindSetting(SettingTag))
            {
                return Found;
            }
        }
        return nullptr;
    }

    // Get all settings as a flat array with their tags
    TMap<FGameplayTag, FMCore_SettingDefinition> GetAllSettings() const
    {
        TMap<FGameplayTag, FMCore_SettingDefinition> AllSettings;
        for (const FMCore_SettingCategory& Category : Categories)
        {
            for (const FMCore_SettingDefinition& Setting : Category.Settings)
            {
                if (Setting.SettingTag.IsValid())
                {
                    AllSettings.Add(Setting.SettingTag, Setting);
                }
            }
        }
        return AllSettings;
    }
};

/**
 * Represents a category of key bindings grouped by display category.
 * Required for Blueprint compatibility (TMap<FName, TArray<...>> not supported in Blueprint).
 */
USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_KeyBindingCategory
{
    GENERATED_BODY()
    
    /** Display name for this category (e.g., "Movement", "Combat", "Inventory") */
    UPROPERTY(BlueprintReadWrite, Category = "Key Bindings")
    FText CategoryName;
    
    /** All key binding settings in this category */
    UPROPERTY(BlueprintReadWrite, Category = "Key Bindings")
    TArray<FMCore_SettingDefinition> KeyBindings;
    
    FMCore_KeyBindingCategory()
        : CategoryName(FText::GetEmpty())
    {}
};