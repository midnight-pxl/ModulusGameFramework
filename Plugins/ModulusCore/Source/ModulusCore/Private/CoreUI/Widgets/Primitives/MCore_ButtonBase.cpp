// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUI/Widgets/Primitives/MCore_ButtonBase.h"
#include "CoreUI/MCore_UISubsystem.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/Texture2D.h"

UMCore_ButtonBase::UMCore_ButtonBase()
{
	// Default text is empty
	ButtonText = FText::GetEmpty();
}

void UMCore_ButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Sync properties to widgets for design-time preview
	SyncPropertiesToWidgets();
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
		}
	}

	// Sync design-time properties to runtime widgets
	SyncPropertiesToWidgets();
}

void UMCore_ButtonBase::NativeDestruct()
{
	UnbindThemeDelegate();

	Super::NativeDestruct();
}

void UMCore_ButtonBase::SetButtonText(const FText& InText)
{
	ButtonText = InText;

	if (Txt_BtnLabel)
	{
		Txt_BtnLabel->SetText(InText);
	}
}

FText UMCore_ButtonBase::GetButtonText() const
{
	return ButtonText;
}

void UMCore_ButtonBase::SetTextJustification(EMCore_TextJustify InJustification)
{
	TextJustification = InJustification;

	if (Txt_BtnLabel)
	{
		ETextJustify::Type SlateJustify;
		switch (InJustification)
		{
		case EMCore_TextJustify::Left:
			SlateJustify = ETextJustify::Left;
			break;
		case EMCore_TextJustify::Right:
			SlateJustify = ETextJustify::Right;
			break;
		case EMCore_TextJustify::Center:
		default:
			SlateJustify = ETextJustify::Center;
			break;
		}
		Txt_BtnLabel->SetJustification(SlateJustify);
	}
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

void UMCore_ButtonBase::SyncPropertiesToWidgets()
{
	// Sync text
	if (Txt_BtnLabel && !ButtonText.IsEmpty())
	{
		Txt_BtnLabel->SetText(ButtonText);
	}

	// Sync justification
	SetTextJustification(TextJustification);

	// Sync display mode
	SetDisplayMode(DisplayMode);
}
