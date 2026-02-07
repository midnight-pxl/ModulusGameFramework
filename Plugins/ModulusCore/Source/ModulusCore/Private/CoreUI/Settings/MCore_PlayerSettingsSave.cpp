// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved.

#include "CoreUI/Settings/MCore_PlayerSettingsSave.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/UserInterfaceSettings.h"
#include "Engine/Engine.h"

UMCore_PlayerSettingsSave::UMCore_PlayerSettingsSave()
{
}

// ============================================================================
// GENERIC ACCESSORS
// ============================================================================

bool UMCore_PlayerSettingsSave::GetFloatSetting(const FString& Key, float& OutValue) const
{
	if (const float* FoundSetting = FloatSettings.Find(Key))
	{
		OutValue = *FoundSetting;
		return true;
	}
	return false;
}

void UMCore_PlayerSettingsSave::SetFloatSetting(const FString& Key, float Value)
{
	FloatSettings.Add(Key, Value);
}

bool UMCore_PlayerSettingsSave::GetIntSetting(const FString& Key, int32& OutValue) const
{
	if (const int32* FoundSetting = IntSettings.Find(Key))
	{
		OutValue = *FoundSetting;
		return true;
	}
	return false;
}

void UMCore_PlayerSettingsSave::SetIntSetting(const FString& Key, int32 Value)
{
	IntSettings.Add(Key, Value);
}

bool UMCore_PlayerSettingsSave::GetBoolSetting(const FString& Key, bool& OutValue) const
{
	if (const bool* FoundSetting = BoolSettings.Find(Key))
	{
		OutValue = *FoundSetting;
		return true;
	}
	return false;
}

void UMCore_PlayerSettingsSave::SetBoolSetting(const FString& Key, bool Value)
{
	BoolSettings.Add(Key, Value);
}

bool UMCore_PlayerSettingsSave::GetKeySetting(const FString& Key, FKey& OutValue) const
{
	if (const FKey* FoundSetting = KeySettings.Find(Key))
	{
		OutValue = *FoundSetting;
		return true;
	}
	return false;
}

void UMCore_PlayerSettingsSave::SetKeySetting(const FString& Key, const FKey& Value)
{
	KeySettings.Add(Key, Value);
}

// ============================================================================
// PENDING SETTERS (Internal — called by UMCore_GameSettingsLibrary)
// ============================================================================


void UMCore_PlayerSettingsSave::SetPendingFloat(const FString& Key, float Value)
{
	PendingFloatSettings.Add(Key, Value);
}

void UMCore_PlayerSettingsSave::SetPendingInt(const FString& Key, int32 Value)
{
	PendingIntSettings.Add(Key, Value);
}

void UMCore_PlayerSettingsSave::SetPendingBool(const FString& Key, bool Value)
{
	PendingBoolSettings.Add(Key, Value);
}

void UMCore_PlayerSettingsSave::SetPendingKey(const FString& Key, const FKey& Value)
{
	PendingKeySettings.Add(Key, Value);
}

// ============================================================================
// PENDING CHANGE MANAGEMENT
// ============================================================================

void UMCore_PlayerSettingsSave::CommitPendingSettings()
{
	for (const auto& [Key, Value] : PendingFloatSettings)
	{
		FloatSettings.Add(Key, Value);
	}
	
	for (const auto& [Key, Value] : PendingIntSettings)
	{
		IntSettings.Add(Key, Value);
	}
	
	for (const auto& [Key, Value] : PendingBoolSettings)
	{
		BoolSettings.Add(Key, Value);
	}
	
	for (const auto& [Key, Value] : PendingKeySettings)
	{
		KeySettings.Add(Key, Value);
	}
	
	PendingFloatSettings.Empty();
	PendingIntSettings.Empty();
	PendingBoolSettings.Empty();
	PendingKeySettings.Empty();
}

void UMCore_PlayerSettingsSave::DiscardPendingSettings()
{
	PendingFloatSettings.Empty();
	PendingIntSettings.Empty();
	PendingBoolSettings.Empty();
	PendingKeySettings.Empty();
}

bool UMCore_PlayerSettingsSave::HasPendingChanges() const
{
	return !PendingFloatSettings.IsEmpty()
	|| !PendingIntSettings.IsEmpty()
	|| !PendingBoolSettings.IsEmpty()
	|| !PendingKeySettings.IsEmpty();
}

// ============================================================================
// PERSISTENCE
// ============================================================================

void UMCore_PlayerSettingsSave::SaveSettings()
{
	UGameplayStatics::SaveGameToSlot(this, GetSaveSlotName(), GetUserIndex());
}

UMCore_PlayerSettingsSave* UMCore_PlayerSettingsSave::LoadPlayerSettings()
{
	if (UGameplayStatics::DoesSaveGameExist(GetSaveSlotName(), GetUserIndex()))
	{
		if (USaveGame* LoadedSave = UGameplayStatics::LoadGameFromSlot(GetSaveSlotName(), GetUserIndex()))
		{
			if (UMCore_PlayerSettingsSave* Settings = Cast<UMCore_PlayerSettingsSave>(LoadedSave))
			{
				Settings->ValidateSettings();
				return Settings;
			}
		}
	}

	/** No save found or cast failed — create fresh defaults */
	return Cast<UMCore_PlayerSettingsSave>(
		UGameplayStatics::CreateSaveGameObject(UMCore_PlayerSettingsSave::StaticClass()));
}

void UMCore_PlayerSettingsSave::LoadPlayerSettingsAsync(FOnPlayerSettingsLoaded OnLoaded)
{
	if (!UGameplayStatics::DoesSaveGameExist(GetSaveSlotName(), GetUserIndex()))
	{
		/** No save on disk — create defaults synchronously and fire callback */
		UMCore_PlayerSettingsSave* NewSettings = Cast<UMCore_PlayerSettingsSave>(
			UGameplayStatics::CreateSaveGameObject(UMCore_PlayerSettingsSave::StaticClass()));
		OnLoaded.ExecuteIfBound(NewSettings);
		return;
	}

	FAsyncLoadGameFromSlotDelegate AsyncLoadGameDelegate;
	AsyncLoadGameDelegate.BindLambda([OnLoaded](const FString& SlotName, const int32 UserIndex, USaveGame* LoadedSave)
	{
		UMCore_PlayerSettingsSave* Settings = nullptr;

		if (LoadedSave)
		{
			Settings = Cast<UMCore_PlayerSettingsSave>(LoadedSave);
		}

		if (!Settings)
		{
			Settings = Cast<UMCore_PlayerSettingsSave>(
				UGameplayStatics::CreateSaveGameObject(UMCore_PlayerSettingsSave::StaticClass()));
		}

		Settings->ValidateSettings();
		OnLoaded.ExecuteIfBound(Settings);
	});

	UGameplayStatics::AsyncLoadGameFromSlot(GetSaveSlotName(), GetUserIndex(), AsyncLoadGameDelegate);
}

// ============================================================================
// FRAMEWORK CONVENIENCE
// ============================================================================

void UMCore_PlayerSettingsSave::SetUIScale(float NewScale)
{
	UIScale = FMath::Clamp(NewScale, 0.5f, 3.0f);
	ApplyUIScale();
}

void UMCore_PlayerSettingsSave::SetTooltipDelay(int32 DelayMs)
{
	TooltipDelayMs = FMath::Max(0, DelayMs);
}

void UMCore_PlayerSettingsSave::SetCategoryCollapsed(const FGameplayTag& CategoryTag, bool bCollapsed)
{
	if (!CategoryTag.IsValid())
	{
		return;
	}

	if (bCollapsed)
	{
		CollapsedCategories.Add(CategoryTag);
	}
	else
	{
		CollapsedCategories.Remove(CategoryTag);
	}
}

bool UMCore_PlayerSettingsSave::IsCategoryCollapsed(const FGameplayTag& CategoryTag) const
{
	return CategoryTag.IsValid() && CollapsedCategories.Contains(CategoryTag);
}

void UMCore_PlayerSettingsSave::SetLastSelectedCategory(const FGameplayTag& CategoryTag)
{
	LastSelectedCategory = CategoryTag;
}

void UMCore_PlayerSettingsSave::ValidateSettings()
{
	UIScale = FMath::Clamp(UIScale, 0.5f, 3.0f);
	TooltipDelayMs = FMath::Max(0, TooltipDelayMs);
}

// ============================================================================
// PRIVATE
// ============================================================================

void UMCore_PlayerSettingsSave::ApplyUIScale()
{
	if (!GEngine) { return; }
	
	UUserInterfaceSettings* UISettings = GetMutableDefault<UUserInterfaceSettings>();
	if (!UISettings) { return; }

	UISettings->ApplicationScale = UIScale;
}