// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUISystem/CoreGameSettings/MCore_GameSettingsSubsystem.h"

#include "CoreData/CoreLogging/LogModulusUI.h"

void UMCore_GameSettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	LoadAllSettings();

	UE_LOG(LogModulusUI, Log, TEXT("MCore_GameSettingsSubsystem initialized with %d settings"),
		CurrentSettings.Num());
	
}

void UMCore_GameSettingsSubsystem::Deinitialize()
{
	SaveAllSettings();
	
	Super::Deinitialize();
}

FString UMCore_GameSettingsSubsystem::GetSettingValue(const FGameplayTag& SaveKey, const FString& SettingValue)
{
	if (const FString* CurrentValue = CurrentSettings.Find(SaveKey)) return CurrentValue;

	if (!SettingValue.IsEmpty())
	{
		CurrentSettings.Add(SaveKey, SettingValue);
	}

	return SettingValue;
}

void UMCore_GameSettingsSubsystem::SetSettingValue(const FGameplayTag& SaveKey, const FString& NewSettingValue,
	bool bSaveImmediately)
{
	const FString CurrentValue = GetSettingValue(SaveKey, "");

	if (CurrentValue != NewSettingValue)
	{
		CurrentSettings.Add(SaveKey, NewSettingValue);
	}

	OnSettingValueChanged(SaveKey, NewSettingValue);
	
	if (bSaveImmediately) SaveToPlayerPrefs();
}

float UMCore_GameSettingsSubsystem::GetFloatSetting(const FGameplayTag& SaveKey, float SettingValue)
{
}

void UMCore_GameSettingsSubsystem::SetFloatSetting(const FGameplayTag& SaveKey, float NewSettingValue)
{
}

bool UMCore_GameSettingsSubsystem::GetBoolSetting(const FGameplayTag& SaveKey, bool SettingValue)
{
}

void UMCore_GameSettingsSubsystem::SetBoolSetting(const FGameplayTag& SaveKey, bool NewSettingValue)
{
}

int32 UMCore_GameSettingsSubsystem::GetIntSetting(const FGameplayTag& SaveKey, int32 DefaultValue)
{
}

void UMCore_GameSettingsSubsystem::SetIntSetting(const FGameplayTag& SaveKey, int32 NewSettingValue)
{
}

TMap<FGameplayTag, FString> UMCore_GameSettingsSubsystem::GetMultipleSettings(const TArray<FGameplayTag>& SaveKeys)
{
}

void UMCore_GameSettingsSubsystem::SetMultipleSettings(const TMap<FGameplayTag, FString>& Settings,
	bool bSaveImmediately)
{
}

bool UMCore_GameSettingsSubsystem::HasSetting(const FGameplayTag& SaveKey) const
{
}

void UMCore_GameSettingsSubsystem::SaveAllSettings()
{
}

void UMCore_GameSettingsSubsystem::LoadAllSettings()
{
}

void UMCore_GameSettingsSubsystem::ApplySettingImmediately(const FGameplayTag& SaveKey, const FString& NewSettingValue)
{
}

void UMCore_GameSettingsSubsystem::ResetAllSettings()
{
}

void UMCore_GameSettingsSubsystem::ResetCategorySettings(const FGameplayTag& CategoryTag)
{
}

void UMCore_GameSettingsSubsystem::OnSettingValueChanged(const FGameplayTag& SaveKey, const FString& NewSettingValue)
{
}

void UMCore_GameSettingsSubsystem::SaveToPlayerPrefs()
{
}

void UMCore_GameSettingsSubsystem::LoadFromPlayerPrefs()
{
}
