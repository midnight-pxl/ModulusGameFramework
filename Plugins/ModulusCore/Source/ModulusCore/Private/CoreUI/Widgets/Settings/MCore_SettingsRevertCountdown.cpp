// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUI/Widgets/Settings/MCore_SettingsRevertCountdown.h"

#include "CoreUI/Widgets/Primitives/MCore_ButtonBase.h"
#include "CoreData/Logging/LogModulusSettings.h"
#include "CoreData/Libraries/MCore_GameSettingsLibrary.h"
#include "CoreData/Types/Settings/MCore_DA_SettingDefinition.h"
#include "CoreData/DevSettings/MCore_CoreSettings.h"

#include "CommonTextBlock.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMCore_SettingsRevertCountdown::UMCore_SettingsRevertCountdown(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldFocusOnActivation = true;
}

// ============================================================================
// LIFECYCLE
// ============================================================================

void UMCore_SettingsRevertCountdown::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Btn_Confirm)
	{
		Btn_Confirm->OnButtonClicked.AddDynamic(this, &ThisClass::HandleConfirmClicked);
	}

	if (Btn_Revert)
	{
		Btn_Revert->OnButtonClicked.AddDynamic(this, &ThisClass::HandleRevertClicked);
	}
}

void UMCore_SettingsRevertCountdown::NativeOnDeactivated()
{
	/** Always kill timer to prevent ticking after deactivation */
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(CountdownTimerHandle);
	}
	
	if (!bResolved)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("SettingsRevertCountdown::NativeOnDeactivated -- deactivated w/o resolution, auto-reverting"));
		
		ApplySettings(false);
		OnCountdownResult.Broadcast(false);
	}
	
	/** Clear external delegates to prevent stale references */
	OnCountdownResult.Clear();
	
	/** Reset state */
	PendingTags.Empty();
	PendingPreviousValues.Empty();
	RemainingSeconds = 0.f;
	TotalSeconds = 0.f;
	
	Super::NativeOnDeactivated();
}

void UMCore_SettingsRevertCountdown::NativeDestruct()
{
	/**
	 * Defensive timer clear (NativeOnDeactivated should've handled this)
	 * Just in case guarantee
	 */
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(CountdownTimerHandle);
	}
	
	/** Clear external delegate in case NativeOnDeactivated was skipped somehow */
	OnCountdownResult.Clear();
	
	if (Btn_Confirm) { Btn_Confirm->OnButtonClicked.RemoveAll(this); }

	if (Btn_Revert)	{ Btn_Revert->OnButtonClicked.RemoveAll(this); }

	Super::NativeDestruct();
}

UWidget* UMCore_SettingsRevertCountdown::NativeGetDesiredFocusTarget() const
{
	/* Default focus to Revert for safety (timer auto-reverts anyway) */
	return Btn_Revert;
}

// ============================================================================
// COUNTDOWN
// ============================================================================

void UMCore_SettingsRevertCountdown::StartCountdown(float Seconds,
	const TArray<FGameplayTag>& AffectedTags,
	const TArray<FString>& PreviousValues)
{
	bResolved = false;
	
	PendingTags = AffectedTags;
	PendingPreviousValues = PreviousValues;
	TotalSeconds = Seconds;
	RemainingSeconds = Seconds;

	if (Txt_Countdown)
	{
		Txt_Countdown->SetText(FText::AsNumber(FMath::CeilToInt(RemainingSeconds)));
	}

	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(
			CountdownTimerHandle, this,
			&ThisClass::HandleCountdownTick, 1.0f, true);
	}

	UE_LOG(LogModulusSettings, Log,
		TEXT("SettingsRevertCountdown::StartCountdown -- started: %.0fs, %d setting(s), widget=%s"),
		TotalSeconds, PendingTags.Num(), *GetNameSafe(this));
}

void UMCore_SettingsRevertCountdown::HandleCountdownTick()
{
	RemainingSeconds -= 1.0f;

	if (Txt_Countdown)
	{
		Txt_Countdown->SetText(FText::AsNumber(FMath::CeilToInt(RemainingSeconds)));
	}

	if (RemainingSeconds <= 0.f)
	{
		ResolveCountdown(false);
	}
}

// ============================================================================
// RESOLUTION
// ============================================================================

void UMCore_SettingsRevertCountdown::HandleConfirmClicked()
{
	ResolveCountdown(true);
}

void UMCore_SettingsRevertCountdown::HandleRevertClicked()
{
	ResolveCountdown(false);
}

void UMCore_SettingsRevertCountdown::ResolveCountdown(bool bConfirmed)
{
	if (bResolved) { return; }
	
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(CountdownTimerHandle);
	}

	ApplySettings(bConfirmed);

	UE_LOG(LogModulusSettings, Log,
		TEXT("SettingsRevertCountdown::ResolveCountdown -- resolved: %s, widget=%s"),
		bConfirmed ? TEXT("Confirmed") : TEXT("Reverted"),
		*GetNameSafe(this));

	OnCountdownResult.Broadcast(bConfirmed);
	DeactivateWidget();
}

void UMCore_SettingsRevertCountdown::ApplySettings(bool bConfirmed)
{
	const UMCore_CoreSettings* CoreSettings = UMCore_CoreSettings::Get();
	if (!CoreSettings)
	{
		UE_LOG(LogModulusSettings, Warning,
			TEXT("SettingsRevertCountdown::ApplySettings -- CoreSettings unavailable, widget=%s"), *GetNameSafe(this));
		return;
	}

	TArray<FMCore_FloatSettingChange> FloatChanges;
	TArray<FMCore_IntSettingChange> IntChanges;
	TArray<FMCore_BoolSettingChange> BoolChanges;

	for (int32 i = 0; i < PendingTags.Num(); ++i)
	{
		UMCore_DA_SettingDefinition* Definition = CoreSettings->FindSettingDefinitionByTag(PendingTags[i]);
		if (!Definition)
		{
			UE_LOG(LogModulusSettings, Warning,
				TEXT("SettingsRevertCountdown::ApplySettings -- no definition found for tag %s, widget=%s"),
				*PendingTags[i].ToString(), *GetNameSafe(this));
			continue;
		}

		switch (Definition->SettingType)
		{
		case EMCore_SettingType::Slider:
		{
			FMCore_FloatSettingChange Change;
			Change.Setting = Definition;
			Change.Value = bConfirmed
				? UMCore_GameSettingsLibrary::GetSettingFloat(this, Definition)
				: FCString::Atof(*PendingPreviousValues[i]);
			FloatChanges.Add(Change);
			break;
		}
		case EMCore_SettingType::Dropdown:
		{
			FMCore_IntSettingChange Change;
			Change.Setting = Definition;
			Change.Value = bConfirmed
				? UMCore_GameSettingsLibrary::GetSettingInt(this, Definition)
				: FCString::Atoi(*PendingPreviousValues[i]);
			IntChanges.Add(Change);
			break;
		}
		case EMCore_SettingType::Toggle:
		{
			FMCore_BoolSettingChange Change;
			Change.Setting = Definition;
			Change.Value = bConfirmed
				? UMCore_GameSettingsLibrary::GetSettingBool(this, Definition)
				: PendingPreviousValues[i].ToBool();
			BoolChanges.Add(Change);
			break;
		}
		}
	}

	/* Re-apply with bypass so the setters save to disk and broadcast */
	if (!FloatChanges.IsEmpty())
	{
		UMCore_GameSettingsLibrary::SetSettingFloat(this, FloatChanges, true);
	}
	if (!IntChanges.IsEmpty())
	{
		UMCore_GameSettingsLibrary::SetSettingInt(this, IntChanges, true);
	}
	if (!BoolChanges.IsEmpty())
	{
		UMCore_GameSettingsLibrary::SetSettingBool(this, BoolChanges, true);
	}
}
