// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_KeyBindingButton.h
 *
 * Single key binding slot button with icon display and key capture via IInputProcessor.
 * Registers a Slate input preprocessor during capture to intercept keys before CommonUI.
 */

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "MCore_KeyBindingButton.generated.h"

class UImage;
class UCommonTextBlock;
class UMCore_ButtonBase;
class UInputAction;
class APlayerController;
class ULocalPlayer;
class IInputProcessor;
class FMCore_KeyCaptureProcessor;

UENUM(BlueprintType)
enum class EMCore_KeyBindingButtonState : uint8
{
	Bound,
	Unbound,
	Capturing
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRebindComplete, bool, bSuccess, FText, ErrorMessage);

/**
 * Single key binding slot with icon display and inline key capture.
 * Uses a Slate IInputProcessor to intercept keys before CommonUI's action router
 * can consume them during capture mode.
 *
 * Requires BindWidget: Btn_Capture, Img_KeyIcon, Txt_KeyName, Txt_CapturePrompt.
 */
UCLASS(Abstract, Blueprintable, ClassGroup = "ModulusUI", meta = (DisableNativeTick))
class MODULUSCORE_API UMCore_KeyBindingButton : public UCommonUserWidget
{
	GENERATED_BODY()
	
	friend class FMCore_KeyCaptureProcessor;

public:
	// ====================================================================
	// PUBLIC API
	// ====================================================================

	/** Configure this button for a specific action, slot, and device type. */
	UFUNCTION(BlueprintCallable, Category = "UI|KeyBinding")
	void InitForSlot(APlayerController* PC, UInputAction* Action,
		EPlayerMappableKeySlot InSlot, bool bInIsGamepad);

	/** Re-read the current binding and update visuals. */
	UFUNCTION(BlueprintCallable, Category = "UI|KeyBinding")
	void RefreshKeyDisplay();

	/** Begin listening for a key press. Registers the input preprocessor. */
	UFUNCTION(BlueprintCallable, Category = "UI|KeyBinding")
	void EnterCaptureMode();

	/** Stop listening and restore display state. */
	UFUNCTION(BlueprintCallable, Category = "UI|KeyBinding")
	void ExitCaptureMode();

	UPROPERTY(BlueprintAssignable, Category = "UI|KeyBinding")
	FOnRebindComplete OnRebindComplete;

	UPROPERTY(BlueprintReadOnly, Category = "UI|KeyBinding")
	EMCore_KeyBindingButtonState CurrentState = EMCore_KeyBindingButtonState::Unbound;

protected:
	// ====================================================================
	// BIND WIDGETS
	// ====================================================================

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMCore_ButtonBase> Btn_Capture;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Img_KeyIcon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Txt_KeyName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Txt_CapturePrompt;
	
	// ====================================================================
	// HANDLERS
	// ====================================================================

	UFUNCTION()
	void HandleButtonPressed();
	void AttemptRebind(FKey NewKey);

	// ====================================================================
	// INPUT PROCESSOR CALLBACKS
	// ====================================================================

	void OnKeyCaptured(FKey NewKey);
	void OnCaptureCancelled();
	
	// ====================================================================
	// LIFECYCLE
	// ====================================================================

	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	// ====================================================================
	// STATE
	// ====================================================================

	TWeakObjectPtr<APlayerController> OwningPC;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> BoundAction;

	EPlayerMappableKeySlot Slot = EPlayerMappableKeySlot::First;
	bool bIsGamepad{false};

	TSharedPtr<IInputProcessor> KeyCaptureProcessor;
};
