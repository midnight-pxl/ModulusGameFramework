// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/CoreWidgets/WidgetPrimitives/MCore_ContainerBase.h"
#include "CoreUISystem/MCore_UISubsystem.h"
#include "CoreData/CoreDataAssets/UIDAs/MasterThemes/MCore_PDA_UITheme_Base.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/NamedSlot.h"
#include "Engine/Texture2D.h"

UMCore_ContainerBase::UMCore_ContainerBase()
{
	// Defaults
}

void UMCore_ContainerBase::NativeOnInitialized()
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
}

void UMCore_ContainerBase::NativeDestruct()
{
	UnbindThemeDelegate();

	Super::NativeDestruct();
}

void UMCore_ContainerBase::SetBackgroundColor(FLinearColor InColor)
{
	if (Border_Background)
	{
		Border_Background->SetBrushColor(InColor);
	}
}

void UMCore_ContainerBase::SetBackgroundBrush(const FSlateBrush& InBrush)
{
	if (Border_Background)
	{
		Border_Background->SetBrush(InBrush);
	}
}

void UMCore_ContainerBase::SetBackgroundImage(UTexture2D* InTexture)
{
	if (Image_Background)
	{
		if (InTexture)
		{
			Image_Background->SetBrushFromTexture(InTexture);
			Image_Background->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			Image_Background->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UMCore_ContainerBase::SetContentPadding(FMargin InPadding)
{
	if (Border_Background)
	{
		Border_Background->SetPadding(InPadding);
	}
}

void UMCore_ContainerBase::ApplyTheme_Implementation(UMCore_PDA_UITheme_Base* Theme)
{
	// Base implementation - derived classes override for specific styling
	K2_OnThemeApplied(Theme);
}

void UMCore_ContainerBase::HandleThemeChanged(UMCore_PDA_UITheme_Base* NewTheme)
{
	ApplyTheme(NewTheme);
}

void UMCore_ContainerBase::BindThemeDelegate()
{
	if (bThemeDelegateBound) { return; }

	ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
	if (!LocalPlayer) { return; }

	UMCore_UISubsystem* UI = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
	if (!UI) { return; }

	UI->OnThemeChanged.AddDynamic(this, &UMCore_ContainerBase::HandleThemeChanged);
	bThemeDelegateBound = true;
}

void UMCore_ContainerBase::UnbindThemeDelegate()
{
	if (!bThemeDelegateBound)
	{
		bThemeDelegateBound = false;
		return;
	}

	ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
	if (!LocalPlayer)
	{
		bThemeDelegateBound = false;
		return;
	}

	UMCore_UISubsystem* UI = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
	if (UI)
	{
		UI->OnThemeChanged.RemoveDynamic(this, &UMCore_ContainerBase::HandleThemeChanged);
	}

	bThemeDelegateBound = false;
}