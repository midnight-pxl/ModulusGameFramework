// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUI/Settings/MCore_PlayerSettingsSave.h"

UMCore_PlayerSettingsSave::UMCore_PlayerSettingsSave()
{
}

void UMCore_PlayerSettingsSave::SaveSettings()
{
}

UMCore_PlayerSettingsSave* UMCore_PlayerSettingsSave::LoadPlayerSettings()
{
	return nullptr;
}

void UMCore_PlayerSettingsSave::LoadPlayerSettingsAsync(FOnPlayerSettingsLoaded OnLoaded)
{
}

void UMCore_PlayerSettingsSave::SetUIScale(float NewScale)
{
}

void UMCore_PlayerSettingsSave::SetTooltipDelay(int32 DelayMs)
{
}

void UMCore_PlayerSettingsSave::MarkCategoryCollapsed(const FString& CategoryName, bool bCollapsed)
{
}

bool UMCore_PlayerSettingsSave::IsCategoryCollapsed(const FString& CategoryName) const
{
	return false;
}

void UMCore_PlayerSettingsSave::ApplyUIScale()
{
}

void UMCore_PlayerSettingsSave::ValidateSettings()
{
}
