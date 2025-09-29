// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/MCore_PrimaryGameLayout.h"

#include "CoreData/CoreLogging/LogModulusUI.h"

UMCore_PrimaryGameLayout::UMCore_PrimaryGameLayout(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UCommonActivatableWidget* UMCore_PrimaryGameLayout::PushWidgetToLayer(const FGameplayTag& LayerTag,
	TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	return nullptr;
}

void UMCore_PrimaryGameLayout::PushWidgetToLayerAsync(const FGameplayTag& LayerTag,
	TSoftClassPtr<UCommonActivatableWidget> WidgetClass)
{
}

bool UMCore_PrimaryGameLayout::PopWidgetFromLayer(const FGameplayTag& LayerTag)
{
	return false;
}

UCommonActivatableWidgetStack* UMCore_PrimaryGameLayout::GetLayerStack(const FGameplayTag& LayerTag) const
{
	return nullptr;
}

void UMCore_PrimaryGameLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InitializeLayerMap();

	UE_LOG(LogModulusUI, Log, TEXT("PrimaryGameLayout initialized with %d layers"),
		LayerMap.Num());
}

void UMCore_PrimaryGameLayout::InitializeLayerMap()
{
}

void UMCore_PrimaryGameLayout::HandleAsyncWidgetLoaded(const FGameplayTag& LayerTag,
	TSoftClassPtr<UCommonActivatableWidget> WidgetClass)
{
}
