// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUISystem/CoreWidgets/WidgetPrimitives/MCore_ButtonBase.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"

void UMCore_ButtonBase::SetButtonText(const FText& InText)
{
	if (!Text_Label)
	{
  		// Widget not bound in Blueprint - silently ignore
  		return;
	}

	Text_Label->SetText(InText);

	// Show/hide text widget based on whether text is empty
	const bool bShouldShowText = !InText.IsEmpty();
	Text_Label->SetVisibility(bShouldShowText ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UMCore_ButtonBase::SetButtonIcon(UTexture2D* InIcon)
{
	if (!Image_Icon)
	{
  		// Widget not bound in Blueprint - silently ignore
  		return;
	}

	if (InIcon)
	{
  		Image_Icon->SetBrushFromTexture(InIcon);
  		Image_Icon->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
  		Image_Icon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FText UMCore_ButtonBase::GetButtonText() const
{
	if (Text_Label)
	{
  		return Text_Label->GetText();
	}

	return FText::GetEmpty();
}
