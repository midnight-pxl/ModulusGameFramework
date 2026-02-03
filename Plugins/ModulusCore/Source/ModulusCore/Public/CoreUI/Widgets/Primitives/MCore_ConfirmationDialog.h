// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreUI/Widgets/Primitives/MCore_ActivatableBase.h"
#include "MCore_ConfirmationDialog.generated.h"

class UCommonTextBlock;
class UMCore_ButtonBase;

/** Delegate fired when the dialog is resolved */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogResult, bool, bConfirmed);

/**
 * Reusable confirmation dialog with Accept/Back input binding support.
 *
 * Blueprint subclasses provide the visual layout with three required bound widgets:
 * - Txt_DialogMessage: Prompt text
 * - Btn_Confirm: Confirm button
 * - Btn_Cancel: Cancel button
 *
 * On activation, the dialog:
 * 1. Wires button click delegates
 * 2. Registers Accept/Back input bindings (visible in action bar)
 * 3. Applies default button labels from ConfirmText/CancelText
 * 4. Focuses Btn_Cancel (safe default)
 *
 * On resolution (confirm or cancel), broadcasts OnDialogResult and deactivates.
 * Input bindings auto-cleanup via MCore_ActivatableBase::NativeOnDeactivated.
 */
UCLASS(Abstract, Blueprintable, ClassGroup="ModulusUI", meta=(DisableNativeTick))
class MODULUSCORE_API UMCore_ConfirmationDialog : public UMCore_ActivatableBase
{
	GENERATED_BODY()

public:
	UMCore_ConfirmationDialog(const FObjectInitializer& ObjectInitializer);

	/** Set the dialog prompt text */
	UFUNCTION(BlueprintCallable, Category = "UI|Dialog")
	void SetDialogMessage(FText InMessage);

	/** Override button labels at runtime */
	UFUNCTION(BlueprintCallable, Category = "UI|Dialog")
	void SetButtonLabels(FText InConfirmText, FText InCancelText);

	/** Fired when dialog resolves. bConfirmed = true if user confirmed, false if cancelled. */
	UPROPERTY(BlueprintAssignable, Category = "UI|Dialog")
	FOnDialogResult OnDialogResult;

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UUserWidget Interface

	/** Input action for confirm (Accept / Gamepad A / Enter) */
	UPROPERTY(EditDefaultsOnly, Category = "UI|Dialog|Input")
	FDataTableRowHandle AcceptInputAction;

	/** Input action for cancel (Back / Gamepad B / Escape) */
	UPROPERTY(EditDefaultsOnly, Category = "UI|Dialog|Input")
	FDataTableRowHandle BackInputAction;

	/** Default confirm button label */
	UPROPERTY(EditDefaultsOnly, Category = "UI|Dialog")
	FText ConfirmText;

	/** Default cancel button label */
	UPROPERTY(EditDefaultsOnly, Category = "UI|Dialog")
	FText CancelText;

	//~ Begin Bound Widgets (Blueprint provides layout)
	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Txt_DialogMessage;

	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidget))
	TObjectPtr<UMCore_ButtonBase> Btn_Confirm;

	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidget))
	TObjectPtr<UMCore_ButtonBase> Btn_Cancel;
	//~ End Bound Widgets

private:
	/** Resolve the dialog and deactivate */
	void ResolveDialog(bool bConfirmed);

	/** Button click handlers */
	UFUNCTION()
	void HandleConfirmClicked();

	UFUNCTION()
	void HandleCancelClicked();

	/** Input action binding callbacks (FName param required by FInputActionExecutedDelegate) */
	UFUNCTION()
	void HandleConfirmInput(FName ActionName);

	UFUNCTION()
	void HandleCancelInput(FName ActionName);

	/** Binding handles for input action tracking */
	FInputActionBindingHandle AcceptBindingHandle;
	FInputActionBindingHandle BackBindingHandle;
};
