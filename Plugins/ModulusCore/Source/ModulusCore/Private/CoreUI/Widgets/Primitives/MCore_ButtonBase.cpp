// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUI/Widgets/Primitives/MCore_ButtonBase.h"
#include "CoreUI/MCore_UISubsystem.h"
#include "CoreData/Assets/UI/Themes/MCore_PDA_UITheme_Base.h"
#include "CoreData/DevSettings/MCore_CoreSettings.h"
#include "CommonTextBlock.h"
#include "CommonButtonBase.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/Texture2D.h"

UMCore_ButtonBase::UMCore_ButtonBase()
{
	ButtonText = FText::GetEmpty();
}

void UMCore_ButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	/** Apply design-time theme for UMG editor preview. Runtime re-applies from UISubsystem. */
	ApplyTheme(UMCore_CoreSettings::GetDesignTimeTheme());

	SyncPropertiesToWidgets();
}

void UMCore_ButtonBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BindThemeDelegate();

	/** Apply initial theme from UISubsystem */
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (UMCore_UISubsystem* UI = LocalPlayer->GetSubsystem<UMCore_UISubsystem>())
		{
			ApplyTheme(UI->GetActiveTheme());
		}
	}

	SyncPropertiesToWidgets();
}

void UMCore_ButtonBase::NativeOnClicked()
{
	Super::NativeOnClicked();
	OnButtonClicked.Broadcast();
}

void UMCore_ButtonBase::SimulateClick()
{
	HandleButtonClicked();
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

	/** Synchronous load - for async, use StreamableManager pattern */
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

void UMCore_ButtonBase::SetButtonStyleOverride(TSubclassOf<UCommonButtonStyle> InStyle)
{
	ButtonStyleOverride = InStyle;

	/** Re-apply current theme to pick up the override */
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (UMCore_UISubsystem* UI = LocalPlayer->GetSubsystem<UMCore_UISubsystem>())
		{
			ApplyTheme(UI->GetActiveTheme());
		}
	}
}

void UMCore_ButtonBase::SetTextStyleOverride(TSubclassOf<UCommonTextStyle> InStyle)
{
	TextStyleOverride = InStyle;

	/** Re-apply current theme to pick up the override */
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (UMCore_UISubsystem* UI = LocalPlayer->GetSubsystem<UMCore_UISubsystem>())
		{
			ApplyTheme(UI->GetActiveTheme());
		}
	}
}

void UMCore_ButtonBase::ApplyTheme_Implementation(UMCore_PDA_UITheme_Base* Theme)
{
	/** Apply button style: override takes precedence, then theme default */
	TSubclassOf<UCommonButtonStyle> ButtonStyleToApply = ButtonStyleOverride;
	if (!ButtonStyleToApply && Theme)
	{
		ButtonStyleToApply = Theme->PrimaryButtonStyle;
	}
	if (ButtonStyleToApply)
	{
		SetStyle(ButtonStyleToApply);
	}

	/** Apply text style: override takes precedence, then theme default */
	TSubclassOf<UCommonTextStyle> TextStyleToApply = TextStyleOverride;
	if (!TextStyleToApply && Theme)
	{
		TextStyleToApply = Theme->BodyTextStyle;
	}
	if (TextStyleToApply && Txt_BtnLabel)
	{
		Txt_BtnLabel->SetStyle(TextStyleToApply);
	}

	K2_OnThemeApplied(Theme);
}

void UMCore_ButtonBase::HandleThemeChanged(UMCore_PDA_UITheme_Base* NewTheme)
{
	CachedTheme = NewTheme;
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
	if (Txt_BtnLabel && !ButtonText.IsEmpty())
	{
		Txt_BtnLabel->SetText(ButtonText);
	}
	
	SetDisplayMode(DisplayMode);

	/** Sync style overrides for design-time preview */
	if (ButtonStyleOverride)
	{
		SetStyle(ButtonStyleOverride);
	}
	if (TextStyleOverride && Txt_BtnLabel)
	{
		Txt_BtnLabel->SetStyle(TextStyleOverride);
	}
}
