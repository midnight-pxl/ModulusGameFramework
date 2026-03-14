// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

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
 * Settings panel listens to update display state across all widgets.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSettingValueChanged,
    FGameplayTag, SettingTag, const FString&, NewValueString);

/**
 * Abstract base for all setting input widgets (Slider, Toggle, Switcher).
 *
 * Immediate-apply model: every user interaction writes to engine immediately.
 * Provides interface so settings panel can iterate setting widgets and call
 * Reset or query values without needing to know the underlying type.
 *
 * Subclasses (Blueprint or C++) implement OnDefinitionSet to read
 * type-specific DataAsset properties and populate UI controls.
 */
UCLASS(Abstract, Blueprintable, ClassGroup= "ModulusUI", meta = (DisableNativeTick))
class MODULUSCORE_API UMCore_SettingsWidget_Base : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
    // ====================================================================
    // INITIALIZATION
    // ====================================================================

    /** Bind this widget to a setting definition. Populates display text and calls OnDefinitionSet. */
    UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
    void InitFromDefinition(const UMCore_DA_SettingDefinition* InDefinition);

    /** The setting definition driving this widget */
    UFUNCTION(BlueprintPure, Category = "ModulusCore|Settings")
    const UMCore_DA_SettingDefinition* GetSettingDefinition() const { return SettingDefinition; }
    
    /** The setting tag from the bound definition */
    UFUNCTION(BlueprintPure, Category = "ModulusCore|Settings")
    FGameplayTag GetSettingTag() const;

    // ====================================================================
    // VALUE INTERFACE
    // ====================================================================

    /** Reset to DataAsset default value */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ModulusCore|Settings")
    void ResetToDefault();

    /** String representation of current value (for debug/display) */
    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ModulusCore|Settings")
    FString GetValueAsString() const;

    /** Step the setting value left (decrement). Panel calls this for gamepad input forwarding. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ModulusCore|Settings")
    void StepLeft();

    /** Step the setting value right (increment). Panel calls this for gamepad input forwarding. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ModulusCore|Settings")
    void StepRight();

    // ====================================================================
    // EVENTS
    // ====================================================================

    /** Fires when user changes a setting value. Value has already been applied to engine. */
    UPROPERTY(BlueprintAssignable, Category = "ModulusCore|Settings")
    FOnSettingValueChanged OnSettingValueChanged;

protected:
    // ====================================================================
    // SUBCLASS HOOKS
    // ====================================================================

    /** Called after InitFromDefinition — subclass reads type-specific DataAsset properties and populates UI */
    UFUNCTION(BlueprintNativeEvent, Category = "ModulusCore|Settings")
    void OnDefinitionSet(const UMCore_DA_SettingDefinition* Definition);

    /** Notify base that value changed — fires OnSettingValueChanged delegate */
    UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
    void BroadcastValueChanged();

    // ====================================================================
    // BIND WIDGETS (common to all setting types)
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
    // THEME DATA
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
