// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUI/Widgets/KeyBindings/MCore_KeyBindingRow.h"

#include "CoreUI/Widgets/KeyBindings/MCore_KeyBindingButton.h"
#include "CoreData/Libraries/MCore_EnhancedInputDisplay.h"
#include "CoreData/Logging/LogModulusUI.h"

#include "InputAction.h"
#include "CommonTextBlock.h"
#include "Engine/World.h"
#include "TimerManager.h"

// ============================================================================
// LIFECYCLE
// ============================================================================

void UMCore_KeyBindingRow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Btn_KBM_Primary)
	{
		Btn_KBM_Primary->OnRebindComplete.AddDynamic(this, &ThisClass::HandleRebindComplete);
	}
	if (Btn_KBM_Secondary)
	{
		Btn_KBM_Secondary->OnRebindComplete.AddDynamic(this, &ThisClass::HandleRebindComplete);
	}
	if (Btn_Gamepad_Primary)
	{
		Btn_Gamepad_Primary->OnRebindComplete.AddDynamic(this, &ThisClass::HandleRebindComplete);
	}
	if (Btn_Gamepad_Secondary)
	{
		Btn_Gamepad_Secondary->OnRebindComplete.AddDynamic(this, &ThisClass::HandleRebindComplete);
	}
}

void UMCore_KeyBindingRow::NativeDestruct()
{
	if (Btn_KBM_Primary) { Btn_KBM_Primary->OnRebindComplete.RemoveAll(this); }
	if (Btn_KBM_Secondary) { Btn_KBM_Secondary->OnRebindComplete.RemoveAll(this); }
	if (Btn_Gamepad_Primary) { Btn_Gamepad_Primary->OnRebindComplete.RemoveAll(this); }
	if (Btn_Gamepad_Secondary) { Btn_Gamepad_Secondary->OnRebindComplete.RemoveAll(this); }

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ErrorFadeTimerHandle);
	}

	Super::NativeDestruct();
}

// ============================================================================
// PUBLIC API
// ============================================================================

void UMCore_KeyBindingRow::InitFromAction(APlayerController* PC, UInputAction* Action,
	bool bShowSecondary)
{
	OwningPC = PC;
	BoundAction = Action;

	if (Txt_ActionName && PC && Action)
	{
		Txt_ActionName->SetText(
			UMCore_EnhancedInputDisplay::GetActionDisplayName(PC, Action));
	}

	if (Btn_KBM_Primary)
	{
		Btn_KBM_Primary->InitForSlot(PC, Action, EPlayerMappableKeySlot::First, false);
	}
	if (Btn_KBM_Secondary)
	{
		Btn_KBM_Secondary->InitForSlot(PC, Action, EPlayerMappableKeySlot::Second, false);
		Btn_KBM_Secondary->SetVisibility(
			bShowSecondary ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (Btn_Gamepad_Primary)
	{
		Btn_Gamepad_Primary->InitForSlot(PC, Action, EPlayerMappableKeySlot::First, true);
	}
	if (Btn_Gamepad_Secondary)
	{
		Btn_Gamepad_Secondary->InitForSlot(PC, Action, EPlayerMappableKeySlot::Second, true);
		Btn_Gamepad_Secondary->SetVisibility(
			bShowSecondary ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	UE_LOG(LogModulusUI, Verbose, TEXT("KeyBindingRow::InitFromAction -- %s initialized for action [%s]"),
		*GetNameSafe(this), *GetNameSafe(BoundAction));

	if (Txt_ErrorMessage)
	{
		Txt_ErrorMessage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMCore_KeyBindingRow::RefreshDisplay()
{
	if (Btn_KBM_Primary) { Btn_KBM_Primary->RefreshKeyDisplay(); }
	if (Btn_KBM_Secondary) { Btn_KBM_Secondary->RefreshKeyDisplay(); }
	if (Btn_Gamepad_Primary) { Btn_Gamepad_Primary->RefreshKeyDisplay(); }
	if (Btn_Gamepad_Secondary) { Btn_Gamepad_Secondary->RefreshKeyDisplay(); }
}

// ============================================================================
// HANDLERS
// ============================================================================

void UMCore_KeyBindingRow::HandleRebindComplete(bool bSuccess, FText ErrorMessage)
{
	if (!bSuccess && !ErrorMessage.IsEmpty())
	{
		UE_LOG(LogModulusUI, Warning, TEXT("KeyBindingRow::HandleRebindComplete -- %s rebind rejected: %s"),
			*GetNameSafe(this), *ErrorMessage.ToString());
		ShowError(ErrorMessage);
	}
	else if (bSuccess)
	{
		UE_LOG(LogModulusUI, Log, TEXT("KeyBindingRow::HandleRebindComplete -- %s rebind succeeded"),
			*GetNameSafe(this));
	}

	RefreshDisplay();
	OnRowRebindCompleted.Broadcast();
}

void UMCore_KeyBindingRow::ShowError(const FText& ErrorMessage)
{
	if (Txt_ErrorMessage)
	{
		Txt_ErrorMessage->SetText(ErrorMessage);
		Txt_ErrorMessage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ErrorFadeTimerHandle);
		World->GetTimerManager().SetTimer(
			ErrorFadeTimerHandle, this, &ThisClass::HideError, 3.0f, false);
	}
}

void UMCore_KeyBindingRow::HideError()
{
	if (Txt_ErrorMessage)
	{
		Txt_ErrorMessage->SetVisibility(ESlateVisibility::Collapsed);
	}
}
