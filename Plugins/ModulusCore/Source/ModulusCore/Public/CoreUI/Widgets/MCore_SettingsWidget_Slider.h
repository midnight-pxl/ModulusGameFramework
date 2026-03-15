// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_SettingsWidget_Slider.h
 *
 * Settings widget for float/slider-type settings with immediate-apply behavior.
 * Reads range, step, and display format from the bound DataAsset.
 */

#pragma once

#include "CoreMinimal.h"
#include "CoreUI/Widgets/MCore_SettingsWidget_Base.h"
#include "MCore_SettingsWidget_Slider.generated.h"

class USlider;
class UMCore_ButtonBase;

/**
 * Settings widget for float/slider-type settings.
 *
 * Immediate-apply model: slider changes write to engine and save instantly.
 * Reads MinValue, MaxValue, StepSize, SliderDisplayFormat from the
 * bound UMCore_DA_SettingDefinition.
 *
 * Key Features:
 * - Value snapping to configurable step size
 * - Three display formats (raw, percentage, whole number)
 * - Optional step buttons for gamepad/keyboard navigation
 * - Theme-driven slider styling via UMCore_PDA_SliderStyle
 *
 * Blueprint Usage:
 *   Create a Blueprint subclass with required BindWidgets:
 *     Slider_Value (USlider), Txt_ValueDisplay (UCommonTextBlock)
 *   Optional: Btn_StepLeft / Btn_StepRight (UMCore_ButtonBase)
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "ModulusUI", meta = (DisableNativeTick))
class MODULUSCORE_API UMCore_SettingsWidget_Slider : public UMCore_SettingsWidget_Base
{
	GENERATED_BODY()

protected:
	// ====================================================================
	// BIND WIDGETS
	// ====================================================================

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> Slider_Value;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Txt_ValueDisplay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UMCore_ButtonBase> Btn_StepLeft;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UMCore_ButtonBase> Btn_StepRight;

	// ====================================================================
	// OVERRIDES
	// ====================================================================

	virtual void OnDefinitionSet_Implementation(const UMCore_DA_SettingDefinition* Definition) override;
	virtual void ApplyTheme_Implementation(UMCore_PDA_UITheme_Base* NewTheme) override;
	virtual FString GetValueAsString_Implementation() const override;
	virtual void ResetToDefault_Implementation() override;
	virtual void StepLeft_Implementation() override;
	virtual void StepRight_Implementation() override;

	// ====================================================================
	// LIFECYCLE
	// ====================================================================

	virtual void NativeOnInitialized() override;

private:
	// ====================================================================
	// HANDLERS
	// ====================================================================

	UFUNCTION()
	void HandleSliderValueChanged(float RawValue);

	UFUNCTION()
	void HandleStepLeft();

	UFUNCTION()
	void HandleStepRight();

	// ====================================================================
	// HELPERS
	// ====================================================================

	void StepByAmount(float Delta);
	float SnapToStep(float RawValue) const;
	FText FormatValue(float Value) const;
	void ApplyValueToEngine(float SnappedValue);
	void SyncSliderAndDisplay(float SnappedValue);

	// ====================================================================
	// STATE
	// ====================================================================

	/* Re-entrancy guard -- prevents HandleSliderValueChanged from firing during programmatic sets */
	bool bIsUpdatingSlider{false};
};
