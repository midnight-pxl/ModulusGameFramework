// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUISystem/MCore_UISettingsTags.h"

FGameplayTag MCore_UISettingsTags::GetWidgetTypeTag(EMCore_SettingType SettingType)
{
	return FGameplayTag::EmptyTag;
}

FGameplayTag MCore_UISettingsTags::GetCategoryTag(const FString& CategoryName)
{
	return FGameplayTag::EmptyTag;
}

FGameplayTag MCore_UISettingsTags::GenerateCustomUISettingTag(const FString& Category, const FString& UISetting)
{
	return FGameplayTag::EmptyTag;
}
