// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUISystem/CoreWidgets/WidgetPrimitives/MCore_ButtonBase.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"

UMCore_ButtonBase::UMCore_ButtonBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMCore_ButtonBase::SetButtonText(const FText& InText)
{
	if (!Text_ButtonTxt)
	{
  		// Widget not bound in Blueprint - silently ignore
  		return;
	}

	Text_ButtonTxt->SetText(InText);

	// Show/hide text widget based on whether text is empty
	const bool bShouldShowText = !InText.IsEmpty();
	Text_ButtonTxt->SetVisibility(bShouldShowText ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UMCore_ButtonBase::SetButtonIcon(UTexture2D* InIcon)
{
	if (!Image_ButtonIcon)
	{
  		// Widget not bound in Blueprint - silently ignore
  		return;
	}

	if (InIcon)
	{
  		Image_ButtonIcon->SetBrushFromTexture(InIcon);
  		Image_ButtonIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
  		Image_ButtonIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMCore_ButtonBase::SetButtonEnabled(bool bInEnabled)
{
}

void UMCore_ButtonBase::NativeOnClicked()
{
	Super::NativeOnClicked();
}

void UMCore_ButtonBase::ApplyUITheme()
{
}

void UMCore_ButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
}

FText UMCore_ButtonBase::GetButtonText() const
{
	if (Text_ButtonTxt)
	{
  		return Text_ButtonTxt->GetText();
	}

	return FText::GetEmpty();
}
