// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/CoreWidgets/MCore_SettingsWidget_Base.h"

#include "CoreData/CoreDevSettings/MCore_CommonUISettings.h"
#include "CoreUISystem/MCore_UISubsystem.h"

UMCore_SettingsWidget_Base::UMCore_SettingsWidget_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMCore_SettingsWidget_Base::NativePreConstruct()
{
	Super::NativePreConstruct();

#if WITH_EDITOR
	// Placeholder text for previewing
	if (SettingDisplayName.IsEmpty())
	{
		SettingDisplayName = INVTEXT("Setting Name Preview");
	}
#endif

	UpdateVisualState();
}

void UMCore_SettingsWidget_Base::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateVisualState();

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

void UMCore_SettingsWidget_Base::NotifyValueChanged(const FString& NewValue)
{
	/**
	 * Fire generic delegate with string-based value 
	 *
	 * Derived class is responsible for:
	 * Calling UpdateVisualState() when appropriate
	 * Saving value if needed
	 * Broadcasting events if needed */
	
	 OnValueChanged.Broadcast(SettingTag, NewValue);

	if (bBroadcastToEventSystem)
	{
		// TODO: tie in broadcast to modulus event system
	}

	UpdateVisualState();
}

void UMCore_SettingsWidget_Base::UpdateVisualState_Implementation()
{
	// Derived classes override to update their visual appearance
}

UMCore_DA_UITheme_Base* UMCore_SettingsWidget_Base::GetCurrentTheme() const
{
	// Return cached subsystem if still valid
	if (CachedTheme.IsValid()) { return CachedTheme.Get(); }

	if (const UMCore_CommonUISettings* DevSettings = GetDefault<UMCore_CommonUISettings>())
	{
		UMCore_DA_UITheme_Base* CurrentTheme = DevSettings->CurrentThemeAsset.LoadSynchronous();
		CachedTheme = CurrentTheme;
		return CurrentTheme;
	}
	
	return nullptr;
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
