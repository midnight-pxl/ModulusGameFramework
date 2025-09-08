// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreData/CoreDevSettings/MCore_CommonUISettings.h"

UMCore_CommonUISettings::UMCore_CommonUISettings()
{
}

TSoftClassPtr<UUserWidget> UMCore_CommonUISettings::GetWidgetClassForType(EMCore_SettingType SettingType) const
{
	return nullptr;
}
