// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_ConfirmationDialog.h
 *
 * Reusable confirmation dialog with Accept/Back input binding support.
 * Broadcasts OnDialogResult and auto-deactivates on resolution.
 */

#pragma once

#include "CoreMinimal.h"
#include "CoreUI/Widgets/Primitives/MCore_ActivatableBase.h"
#include "MCore_ConfirmationDialog.generated.h"

class UCommonTextBlock;
class UMCore_ButtonBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogResult, bool, bConfirmed);

/**
 * Reusable confirmation dialog with Accept/Back input binding support.
 *
 * Key Features:
 * - Auto-wires button click delegates and input bindings on activation
 * - Defaults focus to Cancel button for safety (prevents accidental confirms)
 * - Input bindings auto-cleanup via MCore_ActivatableBase::NativeOnDeactivated
 *
 * Required widget bindings (BindWidget):
 * - Txt_DialogMessage: Prompt text
 * - Btn_Confirm: Confirm button
 * - Btn_Cancel: Cancel button
 *
 * Blueprint Usage:
 * 1. Create Blueprint extending this class with the three required widgets
 * 2. Set AcceptInputAction and BackInputAction in defaults
 * 3. Bind OnDialogResult to handle user choice
 * 4. Activate via widget stack or PushWidgetToLayer
 */
UCLASS(Abstract, Blueprintable, ClassGroup="ModulusUI", meta=(DisableNativeTick))
class MODULUSCORE_API UMCore_ConfirmationDialog : public UMCore_ActivatableBase
{
	GENERATED_BODY()

public:
	UMCore_ConfirmationDialog(const FObjectInitializer& ObjectInitializer);

	/**
	 * Set the dialog prompt text.
	 *
	 * @param InMessage - Text to display as the dialog prompt
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|Dialog")
	void SetDialogMessage(FText InMessage);

	/**
	 * Override button labels at runtime.
	 *
	 * @param InConfirmText - Label for the confirm button
	 * @param InCancelText - Label for the cancel button
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|Dialog")
	void SetButtonLabels(FText InConfirmText, FText InCancelText);

	/**
	 * Fired when dialog resolves.
	 *
	 * @param bConfirmed - true if user confirmed, false if cancelled
	 */
	UPROPERTY(BlueprintAssignable, Category = "UI|Dialog")
	FOnDialogResult OnDialogResult;

protected:
	// ============================================================================
	// LIFECYCLE
	// ============================================================================

	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

	// ============================================================================
	// CONFIGURATION
	// ============================================================================

	UPROPERTY(EditDefaultsOnly, Category = "UI|Dialog|Input")
	FDataTableRowHandle AcceptInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Dialog|Input")
	FDataTableRowHandle BackInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Dialog")
	FText ConfirmText;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Dialog")
	FText CancelText;

	// ============================================================================
	// WIDGET BINDINGS
	// ============================================================================

	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Txt_DialogMessage;

	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidget))
	TObjectPtr<UMCore_ButtonBase> Btn_Confirm;

	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidget))
	TObjectPtr<UMCore_ButtonBase> Btn_Cancel;

private:
	void ResolveDialog(bool bConfirmed);

	UFUNCTION()
	void HandleConfirmClicked();

	UFUNCTION()
	void HandleCancelClicked();

	UFUNCTION()
	void HandleConfirmInput(FName ActionName);

	UFUNCTION()
	void HandleCancelInput(FName ActionName);

	FInputActionBindingHandle AcceptBindingHandle;
	FInputActionBindingHandle BackBindingHandle;
};
