// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/MCore_UICoordinator.h"

void UMCore_UICoordinator::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

bool UMCore_UICoordinator::TryPushWidgetToLayer(FGameplayTag LayerTag,
	TSubclassOf<UCommonActivatableWidget> WidgetClass, EMCore_UIPriority DesiredPriority, const FString& RequestingPlugin)
{
	return false;
}

bool UMCore_UICoordinator::CanPushWithPriority(FGameplayTag LayerTag, EMCore_UIPriority DesiredPriority) const
{
	return false;
}

class UMCore_PrimaryGameLayout* UMCore_UICoordinator::GetPrimaryGameLayout() const
{
	return nullptr;
}

void UMCore_UICoordinator::SetPrimaryGameLayout(class UMCore_PrimaryGameLayout* Layout)
{
}

void UMCore_UICoordinator::RegisterCustomLayer(FGameplayTag LayerTag, UCommonActivatableWidgetStack* LayerStack)
{
}

void UMCore_UICoordinator::OnWidgetActivated(UMCore_ActivatableWidget* Widget)
{
}

void UMCore_UICoordinator::OnWidgetDeactivated(UMCore_ActivatableWidget* Widget)
{
}

UCommonActivatableWidgetStack* UMCore_UICoordinator::GetLayerStack(FGameplayTag LayerTag) const
{
	return nullptr;
}
