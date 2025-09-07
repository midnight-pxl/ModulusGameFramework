// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/MCore_PrimaryGameLayout.h"

UMCore_PrimaryGameLayout::UMCore_PrimaryGameLayout(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UCommonActivatableWidget* UMCore_PrimaryGameLayout::PushWidgetToLayer(FGameplayTag LayerTag,
	TSubclassOf<UCommonActivatableWidget> WidgetClass, EMCore_UIPriority DesiredPriority, const FString& RequestingPlugin)
{
	return nullptr;
}

void UMCore_PrimaryGameLayout::PushWidgetToLayerAsync(FGameplayTag LayerTag,
	TSoftClassPtr<UCommonActivatableWidget> WidgetClass, EMCore_UIPriority DesiredPriority, const FString& RequestingPlugin,
	bool bSuspendInputUntilComplete)
{
}

bool UMCore_PrimaryGameLayout::PopWidgetFromLayer(FGameplayTag LayerTag)
{
	return false;
}

UCommonActivatableWidgetStack* UMCore_PrimaryGameLayout::GetLayerStack(FGameplayTag LayerTag) const
{
	return nullptr;
}

void UMCore_PrimaryGameLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMCore_PrimaryGameLayout::InitializeLayerMap()
{
}

void UMCore_PrimaryGameLayout::OnWidgetClassLoaded(TSoftClassPtr<UCommonActivatableWidget> WidgetClass,
	FGameplayTag LayerTag, EMCore_UIPriority DesiredPriority, FString RequestingPlugin, bool bSuspendInputUntilComplete)
{
}
