// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_KeyBindingRow.h
 *
 * Single action row in the key binding panel with KBM and Gamepad binding buttons.
 * Each row owns 4 buttons: primary/secondary for KBM and Gamepad.
 */

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MCore_KeyBindingRow.generated.h"

class UCommonTextBlock;
class UMCore_KeyBindingButton;
class UInputAction;
class APlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRowRebindCompleted);

/**
 * Single action row with KBM and Gamepad binding slots.
 * Owns 4 KeyBindingButtons and an error toast that auto-hides after 3 seconds.
 *
 * Requires BindWidget: Txt_ActionName, Btn_KBM_Primary, Btn_KBM_Secondary,
 * Btn_Gamepad_Primary, Btn_Gamepad_Secondary, Txt_ErrorMessage.
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "ModulusUI", meta = (DisableNativeTick))
class MODULUSCORE_API UMCore_KeyBindingRow : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	// ====================================================================
	// PUBLIC API
	// ====================================================================

	/** Configure this row for a specific action. Sets display name and inits all buttons. */
	UFUNCTION(BlueprintCallable, Category = "UI|KeyBinding")
	void InitFromAction(APlayerController* PC, UInputAction* Action, bool bShowSecondary);

	/** Re-read current bindings and update all 4 buttons. */
	UFUNCTION(BlueprintCallable, Category = "UI|KeyBinding")
	void RefreshDisplay();

	UFUNCTION(BlueprintPure, Category = "UI|KeyBinding")
	UInputAction* GetInputAction() const { return BoundAction; }

	/** Fires after any button completes a rebind attempt. Panel binds to this for cross-row refresh. */
	UPROPERTY(BlueprintAssignable, Category = "UI|KeyBinding")
	FOnRowRebindCompleted OnRowRebindCompleted;

protected:

	// ====================================================================
	// BIND WIDGETS
	// ====================================================================

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Txt_ActionName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMCore_KeyBindingButton> Btn_KBM_Primary;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMCore_KeyBindingButton> Btn_KBM_Secondary;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMCore_KeyBindingButton> Btn_Gamepad_Primary;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMCore_KeyBindingButton> Btn_Gamepad_Secondary;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Txt_ErrorMessage;

	// ====================================================================
	// LIFECYCLE
	// ====================================================================

	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:

	// ====================================================================
	// HANDLERS
	// ====================================================================

	UFUNCTION()
	void HandleRebindComplete(bool bSuccess, FText ErrorMessage);

	void ShowError(const FText& ErrorMessage);

	UFUNCTION()
	void HideError();

	// ====================================================================
	// STATE
	// ====================================================================

	TWeakObjectPtr<APlayerController> OwningPC;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> BoundAction;

	FTimerHandle ErrorFadeTimerHandle;
};
