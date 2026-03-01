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
 * Fired when user changes setting value (before Apply).
 * Settings panel listens to track dirty state across all widgets.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSettingValueChanged,
    FGameplayTag, SettingTag, const FString&, NewValueString);

/**
 * Abstract base for all setting input widgets (Slider, Toggle, Switcher).
 *
 * Provides interface so settings panel can iterate
 * collection of setting widgets and call Apply/Revert/Reset without
 * needing to know the underlying type.
 *
 * Subclasses (Blueprint or C++) implement OnDefinitionSet to read
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

    /** Write current value to engine systems via GameSettingsLibrary */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ModulusCore|Settings")
    void ApplySettingValue();

    /** Reload value from PlayerSettingsSave, discard unsaved changes */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ModulusCore|Settings")
    void RevertToSaved();

    /** Reset to DataAsset default value */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ModulusCore|Settings")
    void ResetToDefault();

    /** True if current value differs from last-applied/saved value */
    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ModulusCore|Settings")
    bool IsDirty() const;

    /** String representation of current value (for debug/display) */
    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "ModulusCore|Settings")
    FString GetValueAsString() const;

    // ====================================================================
    // EVENTS
    // ====================================================================

    /** Fires when the user changes the value (before Apply) */
    UPROPERTY(BlueprintAssignable, Category = "ModulusCore|Settings")
    FOnSettingValueChanged OnSettingValueChanged;

protected:
    // ====================================================================
    // SUBCLASS HOOKS
    // ====================================================================

    /** Called after InitFromDefinition — subclass reads type-specific DataAsset properties and populates UI */
    UFUNCTION(BlueprintNativeEvent, Category = "ModulusCore|Settings")
    void OnDefinitionSet(const UMCore_DA_SettingDefinition* Definition);

    /** Notify base that value changed — fires delegate, marks dirty */
    UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
    void BroadcastValueChanged();
    
    /** Call after apply or revert to clear dirty flag */
    UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
    void ClearDirtyFlag();

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
    
    UPROPERTY(Transient, BlueprintReadOnly, Category = "ModulusCore|Settings")
    bool bIsDirty{false};

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

    virtual void NativeOnInitialized() override;
    virtual void NativeDestruct() override;

private:
    UFUNCTION()
    void HandleThemeChanged(UMCore_PDA_UITheme_Base* NewTheme);
    void BindThemeDelegate();
    void UnbindThemeDelegate();
    bool bThemeDelegateBound{false};
};
