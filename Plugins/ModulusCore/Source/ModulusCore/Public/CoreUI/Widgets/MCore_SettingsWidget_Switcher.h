// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreUI/Widgets/MCore_SettingsWidget_Base.h"
#include "MCore_SettingsWidget_Switcher.generated.h"

class UMCore_ButtonBase;

/**
 * Inline option cycling widget for Toggle and Dropdown setting types.
 *
 * Immediate-apply model: every cycle writes to the settings system immediately.
 * Toggle settings use two hardcoded options (Off/On). Dropdown settings copy
 * their option labels from the bound UMCore_DA_SettingDefinition.
 *
 * BindWidgets:
 *   Required: Btn_Previous (UMCore_ButtonBase), Btn_Next (UMCore_ButtonBase),
 *             Txt_CurrentOption (UCommonTextBlock)
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "ModulusUI", meta = (DisableNativeTick))
class MODULUSCORE_API UMCore_SettingsWidget_Switcher : public UMCore_SettingsWidget_Base
{
	GENERATED_BODY()

protected:
	// ====================================================================
	// BIND WIDGETS
	// ====================================================================

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMCore_ButtonBase> Btn_Previous;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMCore_ButtonBase> Btn_Next;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Txt_CurrentOption;

	// ====================================================================
	// SUBCLASS HOOKS (overrides from base)
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
	void HandlePreviousClicked();

	UFUNCTION()
	void HandleNextClicked();

	// ====================================================================
	// HELPERS
	// ====================================================================

	void CycleOption(int32 Direction);
	void UpdateDisplay();
	void ApplyCurrentValue();
	void ReadCurrentValue();

	// ====================================================================
	// STATE
	// ====================================================================

	TArray<FText> Options;
	int32 CurrentIndex{0};
	bool bIsToggleMode{false};
};
