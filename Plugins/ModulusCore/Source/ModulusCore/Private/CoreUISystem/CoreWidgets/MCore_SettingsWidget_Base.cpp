// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/CoreWidgets/MCore_SettingsWidget_Base.h"
#include "CoreUISystem/MCore_UISubsystem.h"

UMCore_SettingsWidget_Base::UMCore_SettingsWidget_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMCore_SettingsWidget_Base::NativeConstruct()
{
	Super::NativeConstruct();

	// Load initial value if we have a valid setting tag
	if (SettingTag.IsValid()) { LoadValue(); }
}

void UMCore_SettingsWidget_Base::InitializeWidget(FGameplayTag InSettingTag, FText InDisplayName, FText InDescription)
{
	SettingTag = InSettingTag;
	SettingDisplayName = InDisplayName;
	SettingDescription = InDescription;

	// Update visuals with new configuration
	UpdateVisualState();
    
	// Load value if we now have a valid tag
	if (SettingTag.IsValid()) { LoadValue(); }
}

void UMCore_SettingsWidget_Base::SetEnabled(bool bInEnable, FText Reason)
{
	bEnabled = bInEnable;
	DisabledReason = Reason;

	// Update visual state to reflect enabled/disabled
	UpdateVisualState();
}

void UMCore_SettingsWidget_Base::LoadValue()
{
	// Derived classes override to load their specific value type
}

void UMCore_SettingsWidget_Base::SaveValue()
{
	// Derived classes override to save their specific value type
}

void UMCore_SettingsWidget_Base::ResetToDefault()
{
	// Derived classes override to reset to their default value
}

bool UMCore_SettingsWidget_Base::IsModifiedFromDefault()
{
	// Derived classes override to compare current vs default value
	// Example: return bCurrentValue != bDefaultValue;
	return false;
}

void UMCore_SettingsWidget_Base::OnValueChangedInternal(const FString& NewValue)
{
	/**
	 * Fire generic delegate with string-based value 
	 *
	 * Derived class is responsible for:
	 * Calling UpdateVisualState() when appropriate
	 * Saving value if needed
	 * Broadcasting events if needed */
	
	 OnValueChanged.Broadcast(SettingTag, NewValue);
}

void UMCore_SettingsWidget_Base::UpdateVisualState_Implementation()
{
	// Derived classes override to update their visual appearance
}

UMCore_UISubsystem* UMCore_SettingsWidget_Base::GetMCoreUISubsystem() const
{
	// Return cached subsystem if still valid
	if (CachedUISubsystem.IsValid()) { return CachedUISubsystem.Get(); }

	if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		CachedUISubsystem = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
	}

	return CachedUISubsystem.Get();
}
