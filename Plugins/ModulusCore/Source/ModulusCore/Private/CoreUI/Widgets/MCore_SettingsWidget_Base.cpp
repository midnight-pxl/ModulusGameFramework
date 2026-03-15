// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUI/Widgets/MCore_SettingsWidget_Base.h"
#include "CoreData/Types/Settings/MCore_DA_SettingDefinition.h"
#include "CoreData/Assets/UI/Themes/MCore_PDA_UITheme_Base.h"
#include "CoreData/Logging/LogModulusUI.h"
#include "CoreUI/MCore_UISubsystem.h"
#include "CommonTextBlock.h"
#include "CoreData/DevSettings/MCore_CoreSettings.h"

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

	OnDefinitionSet(InDefinition);
}

FGameplayTag UMCore_SettingsWidget_Base::GetSettingTag() const
{
	return SettingDefinition ? SettingDefinition->SettingTag : FGameplayTag();
}

// ============================================================================
// VALUE INTERFACE (DEFAULTS)
// ============================================================================

void UMCore_SettingsWidget_Base::ResetToDefault_Implementation()
{
	UE_LOG(LogModulusUI, Warning,
		TEXT("[%s] ResetToDefault not overridden — widget will not reset"),
		*GetName());
}

FString UMCore_SettingsWidget_Base::GetValueAsString_Implementation() const
{
	return TEXT("(not implemented)");
}

void UMCore_SettingsWidget_Base::StepLeft_Implementation()
{
}

void UMCore_SettingsWidget_Base::StepRight_Implementation()
{
}

// ============================================================================
// SUBCLASS SUPPORT
// ============================================================================

void UMCore_SettingsWidget_Base::OnDefinitionSet_Implementation(
	const UMCore_DA_SettingDefinition* Definition)
{
}

void UMCore_SettingsWidget_Base::BroadcastValueChanged()
{
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

// ============================================================================
// THEME
// ============================================================================

void UMCore_SettingsWidget_Base::ApplyTheme_Implementation(UMCore_PDA_UITheme_Base* NewTheme)
{
	if (NewTheme)
	{
		if (Txt_SettingName && NewTheme->BodyTextStyle)
		{
			Txt_SettingName->SetStyle(NewTheme->BodyTextStyle);
		}

		if (Txt_SettingDescription && !NewTheme->CaptionTextStyle.IsEmpty())
		{
			int32 SizeIndex = 0;
			if (const ULocalPlayer* LP = GetOwningLocalPlayer())
			{
				if (const UMCore_UISubsystem* UI = LP->GetSubsystem<UMCore_UISubsystem>())
				{
					SizeIndex = UI->GetActiveTextSizeIndex();
				}
			}

			const TSubclassOf<UCommonTextStyle> ResolvedCaptionStyle =
				NewTheme->CaptionTextStyle.IsValidIndex(SizeIndex)
				? NewTheme->CaptionTextStyle[SizeIndex]
				: NewTheme->CaptionTextStyle[0];

			if (ResolvedCaptionStyle)
			{
				Txt_SettingDescription->SetStyle(ResolvedCaptionStyle);
			}
		}
	}

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

void UMCore_SettingsWidget_Base::NativePreConstruct()
{
	Super::NativePreConstruct();
	ApplyTheme(UMCore_CoreSettings::GetDesignTimeTheme());
}

void UMCore_SettingsWidget_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BindThemeDelegate();

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
