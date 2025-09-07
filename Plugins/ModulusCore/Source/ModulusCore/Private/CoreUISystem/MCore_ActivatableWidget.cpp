// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/MCore_ActivatableWidget.h"

UMCore_ActivatableWidget::UMCore_ActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> UMCore_ActivatableWidget::GetDesiredInputConfig() const
{
	return Super::GetDesiredInputConfig();
}

void UMCore_ActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
}

void UMCore_ActivatableWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}

void UMCore_ActivatableWidget::SetWidgetToFocus(UWidget* Widget)
{
}

class UMCore_UICoordinator* UMCore_ActivatableWidget::GetUICoordinator() const
{
	return nullptr;
}

void UMCore_ActivatableWidget::HandleInitialFocus()
{
}
