// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/CoreWidgets/MCore_SettingsWidget_Base.h"

UMCore_SettingsWidget_Base::UMCore_SettingsWidget_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMCore_SettingsWidget_Base::SetEnabled(bool bInEnable, FText Reason)
{
}

void UMCore_SettingsWidget_Base::InitializeWidget(FGameplayTag InSettingTag, FText InDisplayName, FText InDescription)
{
}

void UMCore_SettingsWidget_Base::UpdateVisualState_Implementation()
{
}

void UMCore_SettingsWidget_Base::NativeConstruct()
{
	Super::NativeConstruct();
}
