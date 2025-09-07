// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/MCore_ActivatableWidget.h"

UMCore_ActivatableWidget::UMCore_ActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsBackHandler = true;
	bIsModal = false;
}

TOptional<FUIInputConfig> UMCore_ActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputMode)
	{
	case EMCore_WidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, EMouseCaptureMode::NoCapture);
        
	case EMCore_WidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, EMouseCaptureMode::CapturePermanently);
        
	case EMCore_WidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
        
	case EMCore_WidgetInputMode::Modal:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::CaptureDuringMouseDown);
	
	default:
		return Super::GetDesiredInputConfig();
	}
}
