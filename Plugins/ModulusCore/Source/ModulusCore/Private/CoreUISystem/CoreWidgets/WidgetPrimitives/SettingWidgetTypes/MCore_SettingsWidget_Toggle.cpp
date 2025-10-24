// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/CoreWidgets/WidgetPrimitives/SettingWidgetTypes/MCore_SettingsWidget_Toggle.h"
#include "CoreUISystem/MCore_UISubsystem.h"

void UMCore_SettingsWidget_Toggle::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind to button click
	if (Btn_ToggleButton)
	{
		Btn_ToggleButton->OnClicked().AddUObject(this, &ThisClass::OnToggleButtonClicked);
	}

	// Load starting value
	LoadValue();
}

void UMCore_SettingsWidget_Toggle::SetToggleValue(bool bNewValue)
{
	if (bCurrentValue == bNewValue) { return; }

	bCurrentValue = bNewValue;

	// Type-specific delegate
	OnToggleValueChanged.Broadcast(SettingTag, bCurrentValue);
	
	// Generic delegate (base class, string-based for compatibility)
	NotifyValueChanged(bCurrentValue ? TEXT("true") : TEXT("false"));
}

void UMCore_SettingsWidget_Toggle::OnToggleButtonClicked()
{
	// Toggle the value
	SetToggleValue(!bCurrentValue);
}

void UMCore_SettingsWidget_Toggle::LoadValue()
{
	if (!SettingTag.IsValid()) { return; }

	if (UGameUserSettings* Settings = GetGameUserSettings())
	{
		// This is where SettingTag should be mapped to a valid UGameUserSettings getter
		// Setting to false as safe default
		bCurrentValue = false;
		UpdateVisualState();
	}
}

void UMCore_SettingsWidget_Toggle::SaveValue()
{
	if (!SettingTag.IsValid()) { return; }

	if (UGameUserSettings* Settings = GetGameUserSettings())
	{
		// Map SettingTag to actual setter
		// Example: Settings->SetVSyncEnabled(bCurrentValue);
		
	}
}

void UMCore_SettingsWidget_Toggle::ResetToDefault()
{
	SetToggleValue(bDefaultValue);
}

bool UMCore_SettingsWidget_Toggle::IsModifiedFromDefault()
{
	return bCurrentValue != bDefaultValue;
}

void UMCore_SettingsWidget_Toggle::UpdateVisualState_Implementation()
{

}