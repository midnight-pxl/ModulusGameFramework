// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/CoreWidgets/WidgetPrimitives/MCore_ButtonBase.h"
#include "CoreUISystem/MCore_UISubsystem.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/Texture2D.h"

UMCore_ButtonBase::UMCore_ButtonBase()
{
	// Set Defaults
}

void UMCore_ButtonBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BindThemeDelegate();

	// Apply initial theme
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (UMCore_UISubsystem* UI = LocalPlayer->GetSubsystem<UMCore_UISubsystem>())
		{
			ApplyTheme(UI->GetActiveTheme());
			SetDisplayMode(DisplayMode);
		}
	}
}

void UMCore_ButtonBase::NativeDestruct()
{
	UnbindThemeDelegate();

	Super::NativeDestruct();
}

void UMCore_ButtonBase::SetButtonText(const FText& InText)
{
	if (Txt_BtnLabel)
	{
		Txt_BtnLabel->SetText(InText);
	}
}

FText UMCore_ButtonBase::GetButtonText() const
{
	if (Txt_BtnLabel)
	{
		return Txt_BtnLabel->GetText();
	}
	return FText::GetEmpty();
}

void UMCore_ButtonBase::SetButtonIcon(UTexture2D* InIcon)
{
	if (Img_BtnIcon)
	{
		if (InIcon)
		{
			Img_BtnIcon->SetBrushFromTexture(InIcon);
			Img_BtnIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			Img_BtnIcon->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UMCore_ButtonBase::SetButtonIconSoft(TSoftObjectPtr<UTexture2D> InIcon)
{
	if (InIcon.IsNull())
	{
		SetButtonIcon(nullptr);
		return;
	}

	// Synchronous load - for async, use StreamableManager pattern
	SetButtonIcon(InIcon.LoadSynchronous());
}

void UMCore_ButtonBase::SetDisplayMode(EMCore_ButtonDisplayMode InMode)
{
	DisplayMode = InMode;
	
	if (Switcher_Content)
	{
		Switcher_Content->SetActiveWidgetIndex(static_cast<int32>(DisplayMode));
	}
}

void UMCore_ButtonBase::ApplyTheme_Implementation(UMCore_PDA_UITheme_Base* Theme)
{
	// Base implementation - derived classes override for specific styling
	// Blueprint can also handle via K2_OnThemeApplied
	K2_OnThemeApplied(Theme);
}

void UMCore_ButtonBase::HandleThemeChanged(UMCore_PDA_UITheme_Base* NewTheme)
{
	ApplyTheme(NewTheme);
}

void UMCore_ButtonBase::BindThemeDelegate()
{
	if (bThemeDelegateBound) { return; }

	ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
	if (!LocalPlayer) { return; }

	UMCore_UISubsystem* UI = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
	if (!UI) { return; }

	UI->OnThemeChanged.AddDynamic(this, &UMCore_ButtonBase::HandleThemeChanged);
	bThemeDelegateBound = true;
}

void UMCore_ButtonBase::UnbindThemeDelegate()
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
		UI->OnThemeChanged.RemoveDynamic(this, &UMCore_ButtonBase::HandleThemeChanged);
	}

	bThemeDelegateBound = false;
}
