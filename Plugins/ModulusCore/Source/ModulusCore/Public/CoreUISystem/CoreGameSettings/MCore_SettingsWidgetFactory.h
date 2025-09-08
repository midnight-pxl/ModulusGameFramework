// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "MCore_GameSettingType.h"
#include "CoreUISystem/MCore_UISettingsTags.h"
#include "CoreUISystem/MCore_ActivatableWidget.h"
#include "MCore_SettingsWidgetFactory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameSettingChanged, const FGameplayTag&, SaveKey, const FString&, NewValue);

/**
 * Widget pool entry with GUID tracking for performance
 */
USTRUCT()
struct MODULUSCORE_API FMCore_WidgetPoolEntry
{
    GENERATED_BODY()

    // Widget instance
    UPROPERTY()
    TWeakObjectPtr<UUserWidget> Widget;

    // Unique identifier for this widget instance
    UPROPERTY()
    FGuid WidgetID;

    // Setting this widget was last configured for (for reuse validation)
    UPROPERTY()
    FGameplayTag LastSettingTag;

    // Widget type tag for pooling organization
    UPROPERTY()
    FGameplayTag WidgetTypeTag;

    // Creation time for pool management
    UPROPERTY()
    double CreationTime;

    FMCore_WidgetPoolEntry()
    {
        WidgetID = FGuid::NewGuid();
        CreationTime = FPlatformTime::Seconds();
    }

    bool IsValid() const
    {
        return Widget.IsValid() && WidgetID.IsValid() && WidgetTypeTag.IsValid();
    }
};

/**
 * Factory class that automatically generates CommonUI widgets from setting definitions.
 * This is where the "15-minute setup" magic happens.
 * Now uses FGameplayTags and FGuids for performance optimization.
 */
UCLASS(BlueprintType)
class MODULUSCORE_API UMCore_SettingsWidgetFactory : public UObject
{
    GENERATED_BODY()

public:

    // =================================================================
    // MAIN FACTORY FUNCTIONS - The 15-minute setup workflow
    // =================================================================

    /**
     * Creates a complete settings menu from a configuration.
     * This is the main "one-click" function developers use.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory", CallInEditor)
    static UCommonActivatableWidget* CreateSettingsMenu(
        const FMCore_SettingsConfiguration& Configuration,
        UObject* Outer = nullptr);

    /**
     * Creates a settings category panel with tabs and settings widgets.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory", CallInEditor)
    static UUserWidget* CreateCategoryPanel(
        const FMCore_SettingCategory& Category,
        UObject* Outer = nullptr);

    /**
     * Creates a single setting widget from a definition.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory", CallInEditor)
    static UUserWidget* CreateSettingWidget(
        const FMCore_SettingDefinition& Setting,
        UObject* Outer = nullptr);

    // =================================================================
    // WIDGET POOL MANAGEMENT (Performance optimization with GUIDs)
    // =================================================================

    /**
     * Gets a pooled widget if available, otherwise creates new one.
     * Uses GUID tracking for efficient widget reuse.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory")
    static UUserWidget* GetOrCreateSettingWidget(
        const FMCore_SettingDefinition& Setting,
        UObject* Outer = nullptr);

    /**
     * Returns a widget to the pool when no longer needed.
     * Uses GUID for fast lookup and validation.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory")
    static bool ReturnWidgetToPool(UUserWidget* Widget, const FGuid& WidgetID);

    /**
     * Returns a widget to pool using the widget reference only (slower lookup).
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory")
    static bool ReturnWidgetToPoolByReference(UUserWidget* Widget);

    /**
     * Clears all pooled widgets (call on level transition).
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory")
    static void ClearWidgetPools();

    /**
     * Get pool statistics for debugging and monitoring.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory", BlueprintPure)
    static TMap<FGameplayTag, int32> GetPoolStatistics();

    // =================================================================
    // TAG-BASED HELPER FUNCTIONS
    // =================================================================

    /**
     * Gets the appropriate widget class for a setting type using tags.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory", BlueprintPure)
    static TSubclassOf<UUserWidget> GetWidgetClassForSettingType(EMCore_SettingType SettingType);

    /**
     * Gets the appropriate widget class using the widget type tag directly.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory", BlueprintPure)
    static TSubclassOf<UUserWidget> GetWidgetClassForTag(const FGameplayTag& WidgetTypeTag);

    /**
     * Validates that a setting definition can create a valid widget.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory", BlueprintPure)
    static bool CanCreateWidgetForSetting(const FMCore_SettingDefinition& Setting);

    /**
     * Gets the current value of a setting from save data (integrates with ModulusMemory when available).
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory")
    static FString GetSettingValue(const FGameplayTag& SaveKey, const FString& DefaultValue = "");

    /**
     * Sets the value of a setting (integrates with ModulusMemory when available).
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory")
    static void SetSettingValue(const FGameplayTag& SaveKey, const FString& Value, bool bSaveImmediately = true);

    // Backward compatibility functions for string-based save keys
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory", meta = (DeprecatedFunction))
    static FString GetSettingValueString(const FString& SaveKey, const FString& DefaultValue = "");

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Factory", meta = (DeprecatedFunction))
    static void SetSettingValueString(const FString& SaveKey, const FString& Value, bool bSaveImmediately = true);

    // Global setting change event (now with gameplay tags)
    UPROPERTY(BlueprintAssignable, Category = "Modulus Settings|Events")
    FOnGameSettingChanged OnGameSettingChangedEvent;

protected:

    // =================================================================
    // WIDGET CREATION IMPLEMENTATION
    // =================================================================

    /**
     * Creates a toggle (boolean) widget.
     */
    static UUserWidget* CreateToggleWidget(const FMCore_SettingDefinition& Setting, UObject* Outer);

    /**
     * Creates a slider (float) widget.
     */
    static UUserWidget* CreateSliderWidget(const FMCore_SettingDefinition& Setting, UObject* Outer);

    /**
     * Creates a dropdown (selection) widget.
     */
    static UUserWidget* CreateDropdownWidget(const FMCore_SettingDefinition& Setting, UObject* Outer);

    /**
     * Creates a text input widget.
     */
    static UUserWidget* CreateTextInputWidget(const FMCore_SettingDefinition& Setting, UObject* Outer);

    /**
     * Creates a key binding widget.
     */
    static UUserWidget* CreateKeyBindingWidget(const FMCore_SettingDefinition& Setting, UObject* Outer);

    /**
     * Creates an action button widget.
     */
    static UUserWidget* CreateActionWidget(const FMCore_SettingDefinition& Setting, UObject* Outer);

    // =================================================================
    // WIDGET CONFIGURATION
    // =================================================================

    /**
     * Configures a widget with setting data after creation.
     */
    static void ConfigureWidget(UUserWidget* Widget, const FMCore_SettingDefinition& Setting);

    /**
     * Sets up event bindings for a setting widget.
     */
    static void SetupWidgetEvents(UUserWidget* Widget, const FMCore_SettingDefinition& Setting);

    // =================================================================
    // ENHANCED POOL MANAGEMENT (Private)
    // =================================================================

private:
    // Widget pools organized by widget type tags for fast lookup
    static TMap<FGameplayTag, TArray<FMCore_WidgetPoolEntry>> WidgetPools;
    
    // GUID to pool entry mapping for fast widget return operations
    static TMap<FGuid, FGameplayTag> WidgetIDToTypeMap;
    
    // Widget pointer to GUID mapping for reference-based returns
    static TMap<TWeakObjectPtr<UUserWidget>, FGuid> WidgetToIDMap;

    // Pool performance statistics
    static TMap<FGameplayTag, int32> PoolHitCounts;
    static TMap<FGameplayTag, int32> PoolMissCounts;

    /**
     * Gets a widget from the pool if available, using tag-based lookup.
     */
    static UUserWidget* GetPooledWidget(const FGameplayTag& WidgetTypeTag, const FMCore_SettingDefinition& Setting, UObject* Outer);

    /**
     * Adds a widget to the appropriate pool with GUID tracking.
     */
    static FGuid AddToPool(UUserWidget* Widget, const FGameplayTag& WidgetTypeTag, const FGameplayTag& SettingTag);

    /**
     * Removes invalid widgets from pools and cleans up mappings.
     */
    static void CleanupPools();

    /**
     * Validates that a pooled widget can be reused for a specific setting.
     */
    static bool CanReuseWidget(const FMCore_WidgetPoolEntry& PoolEntry, const FMCore_SettingDefinition& Setting);

    /**
     * Reset a widget to default state for pooling reuse.
     */
    static void ResetWidgetForReuse(UUserWidget* Widget, const FMCore_SettingDefinition& Setting);
};

/**
 * Base class for all Modulus setting widgets.
 * Provides common functionality and interfaces with GUID tracking.
 */
UCLASS(BlueprintType, Blueprintable)
class MODULUSCORE_API UMCore_SettingWidgetBase : public UUserWidget
{
    GENERATED_BODY()

public:
    UMCore_SettingWidgetBase(const FObjectInitializer& ObjectInitializer);

    /**
     * Initialize the widget with setting data.
     */
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Modulus Settings")
    void InitializeWithSetting(const FMCore_SettingDefinition& Setting);

    /**
     * Set the current value of the setting.
     */
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Modulus Settings")
    void SetSettingValue(const FString& Value);

    /**
     * Get the current value of the setting.
     */
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Modulus Settings")
    FString GetSettingValue() const;

    /**
     * Called when the setting value changes.
     */
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Modulus Settings")
    void OnSettingValueChanged(const FString& NewValue);

    /**
     * Reset the setting to its default value.
     */
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Modulus Settings")
    void ResetToDefault();

    /**
     * Prepare widget for pooling (reset state).
     */
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Modulus Settings")
    void PrepareForPool();

    /**
     * Get the unique widget ID for pool management.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings", BlueprintPure)
    FGuid GetWidgetID() const { return WidgetID; }

    /**
     * Get the setting tag this widget represents.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings", BlueprintPure)
    FGameplayTag GetSettingTag() const { return SettingDefinition.SaveKey; }

    // Setting definition this widget represents
    UPROPERTY(BlueprintReadOnly, Category = "Modulus Settings")
    FMCore_SettingDefinition SettingDefinition;

    // Event fired when value changes (now with gameplay tag)
    UPROPERTY(BlueprintAssignable, Category = "Modulus Settings")
    FOnGameSettingChanged OnValueChangedEvent;

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // Helper function to notify of value changes
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
    void NotifyValueChanged(const FString& NewValue);

private:
    // Unique identifier for this widget instance
    UPROPERTY()
    FGuid WidgetID;

    // Track if this widget is currently pooled
    UPROPERTY()
    bool bIsPooled = false;
};

/**
 * Complete settings menu widget that uses our activatable widget base.
 * This is the main widget developers will push to their Menu layer.
 */
UCLASS(BlueprintType, Blueprintable)
class MODULUSCORE_API UMCore_SettingsMenuWidget : public UMCore_ActivatableWidget
{
    GENERATED_BODY()

public:
    UMCore_SettingsMenuWidget(const FObjectInitializer& ObjectInitializer);

    /**
     * Initialize the menu with a settings configuration.
     */
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Modulus Settings")
    void InitializeWithConfiguration(const FMCore_SettingsConfiguration& Configuration);

    /**
     * Save all current settings.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
    void SaveAllSettings();

    /**
     * Reset all settings to defaults.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
    void ResetAllSettings();

    /**
     * Apply settings without saving (for preview).
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
    void ApplySettings();

    /**
     * Get setting value by tag.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings", BlueprintPure)
    FString GetSettingByTag(const FGameplayTag& SaveKey, const FString& DefaultValue = "") const;

    /**
     * Set setting value by tag.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
    void SetSettingByTag(const FGameplayTag& SaveKey, const FString& Value);

protected:
    virtual void NativeOnActivated() override;
    virtual void NativeOnDeactivated() override;

    // The configuration used to build this menu
    UPROPERTY(BlueprintReadOnly, Category = "Modulus Settings")
    FMCore_SettingsConfiguration MenuConfiguration;

    // All setting widgets in this menu mapped by their save key tags
    UPROPERTY(BlueprintReadOnly, Category = "Modulus Settings")
    TMap<FGameplayTag, UMCore_SettingWidgetBase*> SettingWidgetsByTag;

    // Legacy array for backward compatibility
    UPROPERTY(BlueprintReadOnly, Category = "Modulus Settings")
    TArray<UMCore_SettingWidgetBase*> SettingWidgets;

private:
    // Track widgets by their GUIDs for pool management
    TMap<FGuid, UMCore_SettingWidgetBase*> WidgetsByID;

    void OnSettingChanged(const FGameplayTag& SaveKey, const FString& NewValue);
};

/**
 * Blueprint function library for easy access to settings functionality.
 * Enhanced with tag-based operations.
 */
UCLASS()
class MODULUSCORE_API UMCore_SettingsBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Quick function to create and show a complete settings menu.
     * This is the "one-liner" for basic settings.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings", CallInEditor)
    static UCommonActivatableWidget* ShowDefaultSettingsMenu(UObject* WorldContext);

    /**
     * Create settings menu with custom configuration.
     */
    UFUNCTION(BlueprintCallable, Category = "Modulus Settings", CallInEditor)
    static UCommonActivatableWidget* ShowCustomSettingsMenu(
        UObject* WorldContext, 
        const FMCore_SettingsConfiguration& Configuration);

    // =================================================================
    // TAG-BASED SETTING ACCESS (Primary API)
    // =================================================================

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
    static float GetFloatSettingByTag(const FGameplayTag& SaveKey, float DefaultValue = 0.0f);

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
    static void SetFloatSettingByTag(const FGameplayTag& SaveKey, float Value);

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
    static bool GetBoolSettingByTag(const FGameplayTag& SaveKey, bool DefaultValue = false);

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
    static void SetBoolSettingByTag(const FGameplayTag& SaveKey, bool Value);

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
    static int32 GetIntSettingByTag(const FGameplayTag& SaveKey, int32 DefaultValue = 0);

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings")
    static void SetIntSettingByTag(const FGameplayTag& SaveKey, int32 Value);

    // =================================================================
    // STRING-BASED SETTING ACCESS (Backward Compatibility)
    // =================================================================

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings", meta = (DeprecatedFunction))
    static float GetFloatSetting(const FString& SaveKey, float DefaultValue = 0.0f);

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings", meta = (DeprecatedFunction))
    static void SetFloatSetting(const FString& SaveKey, float Value);

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings", meta = (DeprecatedFunction))
    static bool GetBoolSetting(const FString& SaveKey, bool DefaultValue = false);

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings", meta = (DeprecatedFunction))
    static void SetBoolSetting(const FString& SaveKey, bool Value);

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings", meta = (DeprecatedFunction))
    static int32 GetIntSetting(const FString& SaveKey, int32 DefaultValue = 0);

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings", meta = (DeprecatedFunction))
    static void SetIntSetting(const FString& SaveKey, int32 Value);

    // =================================================================
    // TAG UTILITY FUNCTIONS
    // =================================================================

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings", BlueprintPure)
    static FGameplayTag ConvertStringToSettingTag(const FString& SaveKey);

    UFUNCTION(BlueprintCallable, Category = "Modulus Settings", BlueprintPure)
    static FString ConvertSettingTagToString(const FGameplayTag& SaveKey);
};
