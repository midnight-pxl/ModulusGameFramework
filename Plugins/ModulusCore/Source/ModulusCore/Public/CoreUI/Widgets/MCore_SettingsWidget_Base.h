// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_SettingsWidget_Base.h
 *
 * Abstract base for all setting input widgets (Slider, Toggle, Switcher).
 * Immediate-apply model with theme integration and a uniform value interface.
 */

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "MCore_SettingsWidget_Base.generated.h"

class UMCore_DA_SettingDefinition;
class UMCore_PDA_UITheme_Base;
class UCommonTextBlock;

/**
 * Fired when user changes a setting value.
 * Value has already been applied to engine.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSettingValueChanged,
    FGameplayTag, SettingTag, const FString&, NewValueString);

/**
 * Abstract base for all setting input widgets (Slider, Toggle, Switcher).
 *
 * Immediate-apply model: every user interaction writes to engine immediately.
 * Provides a uniform interface so settings panels can iterate widgets and call
 * Reset or query values without knowing the underlying type.
 *
 * Key Features:
 * - DataAsset-driven initialization via InitFromDefinition
 * - Uniform value interface (ResetToDefault, GetValueAsString, StepLeft/Right)
 * - OnSettingValueChanged delegate for panel-level state coordination
 * - Automatic theme binding and cleanup
 *
 * Blueprint Usage:
 *   Subclass in Blueprint. Override OnDefinitionSet to populate type-specific UI.
 *   The settings panel calls InitFromDefinition to bind a setting at runtime.
 */
UCLASS(Abstract, Blueprintable, ClassGroup= "ModulusUI", meta = (DisableNativeTick))
class MODULUSCORE_API UMCore_SettingsWidget_Base : public UCommonUserWidget
{
	GENERATED_BODY()

public:
    // ====================================================================
    // INITIALIZATION
    // ====================================================================

    /**
     * Bind this widget to a setting definition.
     * Populates display text and calls OnDefinitionSet.
     *
     * @param InDefinition  The setting DataAsset to bind.
     */
    UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
    void InitFromDefinition(const UMCore_DA_SettingDefinition* InDefinition);

    UFUNCTION(BlueprintPure, Category = "ModulusCore|Settings")
    const UMCore_DA_SettingDefinition* GetSettingDefinition() const { return SettingDefinition; }

    UFUNCTION(BlueprintPure, Category = "ModulusCore|Settings")
    FGameplayTag GetSettingTag() const;

    // ====================================================================
    // VALUE INTERFACE
    // ====================================================================

    /**
     * Reset to DataAsset default value.
     *
     * Blueprint Usage:
     *   Override in subclass to apply the default and update display.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ModulusCore|Settings")
    void ResetToDefault();

    /**
     * String representation of current value (for debug/display).
     *
     * Blueprint Usage:
     *   Override in subclass to return a formatted value string.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ModulusCore|Settings")
    FString GetValueAsString() const;

    /**
     * Step the setting value left (decrement).
     * Panel calls this for gamepad input forwarding.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ModulusCore|Settings")
    void StepLeft();

    /**
     * Step the setting value right (increment).
     * Panel calls this for gamepad input forwarding.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ModulusCore|Settings")
    void StepRight();

    // ====================================================================
    // EVENTS
    // ====================================================================

    UPROPERTY(BlueprintAssignable, Category = "ModulusCore|Settings")
    FOnSettingValueChanged OnSettingValueChanged;

protected:
    // ====================================================================
    // SUBCLASS HOOKS
    // ====================================================================

    /**
     * Called after InitFromDefinition -- subclass reads type-specific
     * DataAsset properties and populates UI controls.
     */
    UFUNCTION(BlueprintNativeEvent, Category = "ModulusCore|Settings")
    void OnDefinitionSet(const UMCore_DA_SettingDefinition* Definition);

    UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
    void BroadcastValueChanged();

    // ====================================================================
    // BIND WIDGETS
    // ====================================================================

    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    TObjectPtr<UCommonTextBlock> Txt_SettingName;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    TObjectPtr<UCommonTextBlock> Txt_SettingDescription;

    // ====================================================================
    // DATA
    // ====================================================================

    UPROPERTY(Transient, BlueprintReadOnly, Category = "ModulusCore|Settings")
    TObjectPtr<const UMCore_DA_SettingDefinition> SettingDefinition;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "ModulusCore|Settings")
    bool bIsSettingEnabled{true};

    // ====================================================================
    // THEME
    // ====================================================================

    UFUNCTION(BlueprintNativeEvent, Category = "Theme")
    void ApplyTheme(UMCore_PDA_UITheme_Base* NewTheme);

    UFUNCTION(BlueprintImplementableEvent, Category = "Theme",
        meta = (DisplayName = "On Theme Applied"))
    void K2_OnThemeApplied(UMCore_PDA_UITheme_Base* Theme);

    UPROPERTY(Transient)
    mutable TWeakObjectPtr<UMCore_PDA_UITheme_Base> CachedTheme;

    virtual void NativePreConstruct() override;
    virtual void NativeOnInitialized() override;
    virtual void NativeDestruct() override;

private:
    UFUNCTION()
    void HandleThemeChanged(UMCore_PDA_UITheme_Base* NewTheme);
    void BindThemeDelegate();
    void UnbindThemeDelegate();
    bool bThemeDelegateBound{false};
};
