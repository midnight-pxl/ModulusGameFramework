// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MCore_ActivatableWidget.h"
#include "MCore_SettingsWidget_Base.generated.h"

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
class MODULUSCORE_API UMCore_SettingsWidget_Base : public UMCore_ActivatableWidget
{
	GENERATED_BODY()

public:
	UMCore_SettingsWidget_Base(const FObjectInitializer& ObjectInitializer);
	
	/** Unique identifier for this setting (e.g., Settings.Audio.MasterVolume) */
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
	
	/**
	 * Reset this setting to its default value
	 * Derived classes should restore default, update UI, and broadcast change
	 */
	UFUNCTION(BlueprintCallable, Category = "Setting Widget")
	virtual void ResetToDefault() PURE_VIRTUAL(UMCore_SettingWidget_Base::ResetToDefault, );
	
	/**
	 * Check if current value differs from default
	 * Used for "unsaved changes" indicators
	 */
	UFUNCTION(BlueprintPure, Category = "Setting Widget")
	virtual bool IsModifiedFromDefault() const PURE_VIRTUAL(UMCore_SettingWidget_Base::IsModifiedFromDefault, return false;);
	
	/**
	 * Enable or disable this setting widget
	 * @param bInEnable - True to enable, false to disable
	 * @param Reason - Optional reason shown in tooltip when disabled
	 */
	UFUNCTION(BlueprintCallable, Category = "Setting Widget")
	void SetEnabled(bool bInEnable, FText Reason = FText::GetEmpty());
	
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
	 * Update widget's visual appearance
	 * Derived classes override to update UI elements (slider position, checkbox state, etc.)
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Setting Widget")
	void UpdateVisualState();
	virtual void UpdateVisualState_Implementation();
	
	virtual void NativeConstruct() override;
};
