// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUI/Widgets/MCore_SettingsWidget_Switcher.h"
#include "CoreData/Types/Settings/MCore_DA_SettingDefinition.h"
#include "CoreData/Types/Settings/MCore_SettingsTypes.h"
#include "CoreData/Libraries/MCore_GameSettingsLibrary.h"
#include "CoreData/Assets/UI/Themes/MCore_PDA_UITheme_Base.h"
#include "CoreData/Logging/LogModulusUI.h"
#include "CoreUI/Widgets/Primitives/MCore_ButtonBase.h"
#include "CommonTextBlock.h"
#include "CoreData/Libraries/MCore_ThemeLibrary.h"

// ============================================================================
// LIFECYCLE
// ============================================================================

void UMCore_SettingsWidget_Switcher::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Btn_Previous)
	{
		Btn_Previous->OnButtonClicked.AddDynamic(this, &ThisClass::HandlePreviousClicked);
	}

	if (Btn_Next)
	{
		Btn_Next->OnButtonClicked.AddDynamic(this, &ThisClass::HandleNextClicked);
	}
}

// ============================================================================
// DEFINITION SET
// ============================================================================

void UMCore_SettingsWidget_Switcher::OnDefinitionSet_Implementation(
	const UMCore_DA_SettingDefinition* Definition)
{
	if (!Definition)
	{
		return;
	}

	bIsToggleMode = (Definition->SettingType == EMCore_SettingType::Toggle);

	if (bIsToggleMode)
	{
		Options.Reset(2);
		Options.Add(NSLOCTEXT("ModulusCore", "ToggleOff", "Off"));
		Options.Add(NSLOCTEXT("ModulusCore", "ToggleOn", "On"));
	}
	else
	{
		Options = Definition->DropdownOptions;
	}

	ReadCurrentValue();
	UpdateDisplay();
}

// ============================================================================
// BUTTON HANDLERS
// ============================================================================

void UMCore_SettingsWidget_Switcher::HandlePreviousClicked()
{
	StepLeft();
}

void UMCore_SettingsWidget_Switcher::HandleNextClicked()
{
	StepRight();
}

// ============================================================================
// STEP OVERRIDES
// ============================================================================

void UMCore_SettingsWidget_Switcher::StepLeft_Implementation()
{
	CycleOption(-1);
}

void UMCore_SettingsWidget_Switcher::StepRight_Implementation()
{
	CycleOption(1);
}

// ============================================================================
// HELPERS
// ============================================================================

void UMCore_SettingsWidget_Switcher::CycleOption(int32 Direction)
{
	if (Options.Num() <= 0)
	{
		return;
	}

	CurrentIndex = (CurrentIndex + Direction + Options.Num()) % Options.Num();
	ApplyCurrentValue();
	UpdateDisplay();
}

void UMCore_SettingsWidget_Switcher::UpdateDisplay()
{
	if (Txt_CurrentOption && Options.IsValidIndex(CurrentIndex))
	{
		Txt_CurrentOption->SetText(Options[CurrentIndex]);
	}
}

void UMCore_SettingsWidget_Switcher::ApplyCurrentValue()
{
	if (!SettingDefinition)
	{
		return;
	}

	UMCore_DA_SettingDefinition* MutableDef =
		const_cast<UMCore_DA_SettingDefinition*>(SettingDefinition.Get());

	if (bIsToggleMode)
	{
		TArray<FMCore_BoolSettingChange> Changes;
		Changes.Add({MutableDef, CurrentIndex == 1});
		UMCore_GameSettingsLibrary::SetSettingBool(GetOwningLocalPlayer(), Changes, false);
	}
	else
	{
		TArray<FMCore_IntSettingChange> Changes;
		Changes.Add({MutableDef, CurrentIndex});
		UMCore_GameSettingsLibrary::SetSettingInt(GetOwningLocalPlayer(), Changes, false);
	}

	BroadcastValueChanged();
}

void UMCore_SettingsWidget_Switcher::ReadCurrentValue()
{
	if (!SettingDefinition) { return; }

	if (bIsToggleMode)
	{
		const bool BoolVal = UMCore_GameSettingsLibrary::GetSettingBool(
			GetOwningLocalPlayer(), SettingDefinition.Get());
		CurrentIndex = BoolVal ? 1 : 0;
	}
	else
	{
		CurrentIndex = UMCore_GameSettingsLibrary::GetSettingInt(
			GetOwningLocalPlayer(), SettingDefinition.Get());
	}

	if (Options.Num() > 0)
	{
		CurrentIndex = FMath::Clamp(CurrentIndex, 0, Options.Num() - 1);
	}
	else
	{
		CurrentIndex = 0;
	}
}

// ============================================================================
// VALUE INTERFACE OVERRIDES
// ============================================================================

FString UMCore_SettingsWidget_Switcher::GetValueAsString_Implementation() const
{
	if (Options.IsValidIndex(CurrentIndex))
	{
		return Options[CurrentIndex].ToString();
	}

	return FString();
}

void UMCore_SettingsWidget_Switcher::ResetToDefault_Implementation()
{
	if (!SettingDefinition) { return; }
	
	UMCore_GameSettingsLibrary::ResetSettingToDefault(
		GetOwningLocalPlayer(), SettingDefinition.Get());

	ReadCurrentValue();
	UpdateDisplay();
}

// ============================================================================
// THEME
// ============================================================================

void UMCore_SettingsWidget_Switcher::ApplyTheme_Implementation(UMCore_PDA_UITheme_Base* NewTheme)
{
	Super::ApplyTheme_Implementation(NewTheme);

	if (!NewTheme) { return; }
	
	UMCore_ThemeLibrary::ApplyTextStyleFromTheme(
		GetOwningLocalPlayer(), Txt_CurrentOption, NewTheme->ValueTextStyle);
	
	const TSubclassOf<UCommonButtonStyle> ArrowStyle =
		UMCore_ThemeLibrary::ResolveButtonStyle(
			NewTheme->GhostButtonStyle, NewTheme->SecondaryButtonStyle);

	if (ArrowStyle)
	{
		Btn_Previous->SetButtonStyleOverride(ArrowStyle);
		Btn_Next->SetButtonStyleOverride(ArrowStyle);
	}
}
