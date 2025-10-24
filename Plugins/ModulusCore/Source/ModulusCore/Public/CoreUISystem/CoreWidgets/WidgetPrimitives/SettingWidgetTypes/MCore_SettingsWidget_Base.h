// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "MCore_SettingsWidget_Base.generated.h"

class UGameUserSettings;
class UMCore_DA_UITheme_Base;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSettingValueChanged,
	FGameplayTag, SettingTag,
	FString, NewValue);

/**
 * Base class for all setting widgets
 *
 * Provides common properties (tag, display name, description) and interface for value management.
 * Derived classes implement type-specific value handling (bool, float, int32, FGameplayTag).
 *
 * Derived Classes:
 * - UMCore_SettingWidget_Slider (volume, brightness, sensitivity)
 * - UMCore_SettingWidget_Toggle (VSync, motion blur, subtitles)
 * - UMCore_SettingWidget_Dropdown (quality presets, resolutions)
 * - UMCore_SettingWidget_KeyBinding (input remapping)
 */
UCLASS(Abstract, Blueprintable)
class MODULUSCORE_API UMCore_SettingsWidget_Base : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UMCore_SettingsWidget_Base(const FObjectInitializer& ObjectInitializer);
	
	/**
	 * Unique identifier for this setting (e.g., Settings.Audio.MasterVolume)
	 * Used for: Save/Load player settings, Cross-plugin queries, Event system broadcasts
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Configuration",
		meta = (Categories = "Settings"))
	FGameplayTag SettingTag;
	
	/** Display name shown to player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Configuration")
	FText SettingDisplayName;
	
	/** Optional description/tooltip text */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Configuration")
	FText SettingDescription;
	
	/** Is this setting currently enabled? Disabled settings are grayed out and non-interactive */
	UPROPERTY(BlueprintReadOnly, Category = "Setting State")
	bool bEnabled = true;

	/** Explanation shown when setting is disabled (e.g., "Requires HDR-capable display") */
	UPROPERTY(BlueprintReadOnly, Category = "Setting State")
	FText DisabledReason;

	/** Should value changes broadcast to Modulus global event system? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Configuration")
	bool bBroadcastToEventSystem = false;

	/**
	 * Delegates
	 */

	/** 
	 * Fires when setting value changes.
	 * Provides generic string-based notification for any setting type.
	 * 
	 * Use this when you:
	 * - Need "something changed" notification (enable Apply button)
	 * - Don't care about the actual value
	 * - Want to handle multiple setting types with one handler
	 * 
	 * For type-safe value access, bind to derived class delegates instead:
	 * - OnToggleChanged, OnSliderChanged, etc.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Setting Events")
	FOnSettingValueChanged OnValueChanged;
	
	/**
	 * Reset this setting to its default value
	 * Derived classes should restore default, update UI, and broadcast change
	 */
	UFUNCTION(BlueprintCallable, Category = "Setting Widget")
	virtual void ResetToDefault();
	
	/**
	 * Check if current value differs from default
	 * Used for "unsaved changes" indicators
	 */
	UFUNCTION(BlueprintPure, Category = "Setting Widget")
	virtual bool IsModifiedFromDefault();
	
	/**
	 * Enable or disable this setting widget
	 * @param bInEnable - True to enable, false to disable
	 * @param Reason - Optional reason shown in tooltip when disabled
	 */
	UFUNCTION(BlueprintCallable, Category = "Setting Widget")
	void SetEnabled(bool bInEnable, FText Reason = FText::GetEmpty());

	/** Load value from game settings (via subsystem) */
	UFUNCTION(BlueprintCallable, Category = "Setting")
	virtual void LoadValue();
    
	/** Save value to game settings (via subsystem) */
	UFUNCTION(BlueprintCallable, Category = "Setting")
	virtual void SaveValue();
	
	/**
	 * Initialize widget with setting info
	 * Called during menu construction or dynamic widget creation
	 */
	UFUNCTION(BlueprintCallable, Category = "Setting Widget")
	void InitializeWidget(FGameplayTag InSettingTag, 
	                     FText InDisplayName, 
	                     FText InDescription = FText::GetEmpty());
	
protected:
	/**
	 * Editor-time preview initialization.
	 * 
	 * Use for visual preview that doesn't require game world:
	 * - Apply default styles
	 * - Set placeholder text
	 * - Show example values
	 * 
	 * DO NOT:
	 * - Access subsystems (no world)
	 * - Load player settings (no game instance)
	 * - Register delegates (no event system)
	 */
	virtual void NativePreConstruct() override;

	/**
	 * Runtime initialization.
	 * 
	 * Runs when widget created during gameplay.
	 * Use for actual game integration:
	 * - Load values from subsystem
	 * - Register event listeners
	 * - Apply runtime theme
	 */
	virtual void NativeConstruct() override;
	
	/** 
	 * Update visual state based on current value and enabled state.
	 * 
	 * Override this to update your widget's visual appearance when:
	 * - Value changes
	 * - Enabled state changes
	 * - Theme is applied
	 * 
	 * Example: Update toggle knob position, slider thumb location, dropdown selection.
	 * 
	 * NOTE: This is NOT called automatically when value changes.
	 * Derived classes must call this explicitly when appropriate.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "SettingWidget")
	void UpdateVisualState();
	virtual void UpdateVisualState_Implementation();
	
    /** 
     * Called when value changes.
     * Fires OnValueChanged delegate with string-based value.
     * 
     * Derived classes should call this after changing their value:
     * - SetValue(bool) → OnValueChangedInternal("true")
     * - SetValue(float) → OnValueChangedInternal(LexToString(Value))
     * 
     * This method ONLY fires the delegate. It does NOT:
     * - Call UpdateVisualState() (derived class controls timing)
     * - Broadcast to event system (settings screen handles if needed)
     * - Save value (derived class or settings screen handles)
     * 
     * @param NewValue - New value as string (for generic delegate)
     */
    virtual void NotifyValueChanged(const FString& NewValue);

	UFUNCTION(BlueprintPure, Category = "SettingHelpers")
	UMCore_DA_UITheme_Base* GetCurrentTheme() const;
	
    /** 
     * Get settings subsystem (cached for performance).
     * Use this to load/save values in derived classes.
     * 
     * @return Settings subsystem, or nullptr if not available
     */
    UFUNCTION(BlueprintPure, Category = "SettingHelpers")
    UGameUserSettings* GetGameUserSettings() const;
    
private:	

	/** Cached theme reference (fetched once at construction) */
	mutable TWeakObjectPtr<UMCore_DA_UITheme_Base> CachedTheme;
};
