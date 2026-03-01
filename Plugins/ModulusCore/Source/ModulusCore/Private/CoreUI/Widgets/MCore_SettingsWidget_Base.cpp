// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUI/Widgets/MCore_SettingsWidget_Base.h"
#include "CoreData/Types/Settings/MCore_DA_SettingDefinition.h"
#include "CoreData/Assets/UI/Themes/MCore_PDA_UITheme_Base.h"
#include "CoreData/Logging/LogModulusUI.h"
#include "CoreUI/MCore_UISubsystem.h"
#include "CommonTextBlock.h"

// ============================================================================
// INITIALIZATION
// ============================================================================

void UMCore_SettingsWidget_Base::InitFromDefinition(const UMCore_DA_SettingDefinition* InDefinition)
{
	if (!InDefinition)
	{
		UE_LOG(LogModulusUI, Warning,
			TEXT("[%s] InitFromDefinition: null definition passed"), *GetName());
		return;
	}

	if (!InDefinition->IsValid())
	{
		UE_LOG(LogModulusUI, Warning,
			TEXT("[%s] InitFromDefinition: definition '%s' failed validation"),
			*GetName(), *InDefinition->GetName());
		return;
	}

	SettingDefinition = InDefinition;
	bIsDirty = false;

	/** Populate common display text */
	if (Txt_SettingName)
	{
		Txt_SettingName->SetText(InDefinition->DisplayName);
	}

	if (Txt_SettingDescription)
	{
		Txt_SettingDescription->SetText(InDefinition->Description);
	}

	UE_LOG(LogModulusUI, Verbose,
		TEXT("[%s] Initialized from definition: %s (%s)"),
		*GetName(),
		*InDefinition->DisplayName.ToString(),
		*InDefinition->SettingTag.ToString());

	/** Let subclass read type-specific properties */
	OnDefinitionSet(InDefinition);
}

FGameplayTag UMCore_SettingsWidget_Base::GetSettingTag() const
{
	return SettingDefinition ? SettingDefinition->SettingTag : FGameplayTag();
}

// ============================================================================
// POLYMORPHIC VALUE INTERFACE (defaults)
// ============================================================================

void UMCore_SettingsWidget_Base::ApplySettingValue_Implementation()
{
	UE_LOG(LogModulusUI, Warning,
		TEXT("[%s] ApplySettingValue not overridden — value will not persist"),
		*GetName());
}

void UMCore_SettingsWidget_Base::RevertToSaved_Implementation()
{
	UE_LOG(LogModulusUI, Warning,
		TEXT("[%s] RevertToSaved not overridden — widget will not refresh"),
		*GetName());
}

void UMCore_SettingsWidget_Base::ResetToDefault_Implementation()
{
	UE_LOG(LogModulusUI, Warning,
		TEXT("[%s] ResetToDefault not overridden — widget will not reset"),
		*GetName());
}

bool UMCore_SettingsWidget_Base::IsDirty_Implementation() const
{
	return bIsDirty;
}

FString UMCore_SettingsWidget_Base::GetValueAsString_Implementation() const
{
	return TEXT("(not implemented)");
}

// ============================================================================
// SUBCLASS SUPPORT
// ============================================================================

void UMCore_SettingsWidget_Base::OnDefinitionSet_Implementation(
	const UMCore_DA_SettingDefinition* Definition)
{
	/** Default no-op — subclass reads type-specific DataAsset properties here */
}

void UMCore_SettingsWidget_Base::BroadcastValueChanged()
{
	bIsDirty = true;

	if (SettingDefinition)
	{
		const FString ValueStr = GetValueAsString();
		OnSettingValueChanged.Broadcast(SettingDefinition->SettingTag, ValueStr);

		UE_LOG(LogModulusUI, Verbose,
			TEXT("[%s] Value changed: %s = %s"),
			*GetName(),
			*SettingDefinition->SettingTag.ToString(),
			*ValueStr);
	}
}

void UMCore_SettingsWidget_Base::ClearDirtyFlag()
{
	bIsDirty = false;
}

// ============================================================================
// THEME
// ============================================================================

void UMCore_SettingsWidget_Base::ApplyTheme_Implementation(UMCore_PDA_UITheme_Base* NewTheme)
{
	K2_OnThemeApplied(NewTheme);
}

void UMCore_SettingsWidget_Base::HandleThemeChanged(UMCore_PDA_UITheme_Base* NewTheme)
{
	CachedTheme = NewTheme;
	ApplyTheme(NewTheme);
}

void UMCore_SettingsWidget_Base::BindThemeDelegate()
{
	if (bThemeDelegateBound) { return; }

	ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
	if (!LocalPlayer) { return; }

	UMCore_UISubsystem* UI = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
	if (!UI) { return; }

	UI->OnThemeChanged.AddDynamic(this, &UMCore_SettingsWidget_Base::HandleThemeChanged);
	bThemeDelegateBound = true;
}

void UMCore_SettingsWidget_Base::UnbindThemeDelegate()
{
	if (!bThemeDelegateBound) { return; }

	ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
	if (!LocalPlayer)
	{
		bThemeDelegateBound = false;
		return;
	}

	UMCore_UISubsystem* UI = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
	if (UI)
	{
		UI->OnThemeChanged.RemoveDynamic(this, &UMCore_SettingsWidget_Base::HandleThemeChanged);
	}

	bThemeDelegateBound = false;
}

// ============================================================================
// LIFECYCLE
// ============================================================================

void UMCore_SettingsWidget_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BindThemeDelegate();

	/** Apply initial theme */
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (UMCore_UISubsystem* UI = LocalPlayer->GetSubsystem<UMCore_UISubsystem>())
		{
			ApplyTheme(UI->GetActiveTheme());
		}
	}
}

void UMCore_SettingsWidget_Base::NativeDestruct()
{
	UnbindThemeDelegate();
	Super::NativeDestruct();
}