// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreData/CoreLibraries/SettingsLibraries/MCore_EnhancedInputDisplay.h"

#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInput/Public/UserSettings/EnhancedInputUserSettings.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMappableInputConfig.h"
#include "GameFramework/PlayerController.h"
#include "CoreData/CoreLogging/LogModulusSettings.h"

UEnhancedInputLocalPlayerSubsystem* UMCore_EnhancedInputDisplay::GetEnhancedInputSubsystem(
	APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		UE_LOG(LogModulusSettings, Error, TEXT("GetEnhancedInputSubsystem: PlayerController is null"));
		return nullptr;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer)
	{
		UE_LOG(LogModulusSettings, Error, TEXT("GetEnhancedInputSubsystem: LocalPlayer is null"));
		return nullptr;
	}

	return LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
}

FKey UMCore_EnhancedInputDisplay::GetCurrentKeyForAction(APlayerController* PlayerController, UInputAction* InputAction)
{
	if (!InputAction) return EKeys::Invalid;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem(PlayerController);
	if (!Subsystem) return EKeys::Invalid;

	UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
	if (!UserSettings) return EKeys::Invalid;

	UEnhancedPlayerMappableKeyProfile* CurrentProfile = UserSettings->GetActiveKeyProfile();
	if (!CurrentProfile) return EKeys::Invalid;

	const TMap<FName, FKeyMappingRow>& KeyMappings = CurrentProfile->GetPlayerMappingRows();
	for (const auto& [RowName, MappingRow] : KeyMappings)
	{
		for (const FPlayerKeyMapping& CurrentKeyMapping : MappingRow.Mappings)
		{
			if (CurrentKeyMapping.GetAssociatedInputAction() == InputAction)
			{
				return CurrentKeyMapping.GetCurrentKey();
			}
		}
	}

	return EKeys::Invalid;
}

TArray<FKey> UMCore_EnhancedInputDisplay::GetAllKeysForAction(APlayerController* PlayerController,
	UInputAction* InputAction)
{
	TArray<FKey> Keys;

	if (!InputAction) return Keys;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem(PlayerController);
	if (!Subsystem) return Keys;

	UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
	if (!UserSettings) return Keys;

	UEnhancedPlayerMappableKeyProfile* CurrentProfile = UserSettings->GetActiveKeyProfile();
	if (!CurrentProfile) return Keys;

	const TMap<FName, FKeyMappingRow>& KeyMappings = CurrentProfile->GetPlayerMappingRows();
	for (const auto& [RowName, MappingRow] : KeyMappings)
	{
		for (const FPlayerKeyMapping& CurrentKeyMapping : MappingRow.Mappings)
		{
			if (CurrentKeyMapping.GetAssociatedInputAction() == InputAction)
			{
				Keys.Add(CurrentKeyMapping.GetCurrentKey());
			}
		}
	}

	return Keys;
}

FText UMCore_EnhancedInputDisplay::GetActionDisplayName(APlayerController* PlayerController, UInputAction* InputAction)
{
	if (!InputAction) { return FText::FromString(TEXT("Invalid InputAction")); }

	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem(PlayerController);
	if (Subsystem)
	{
		UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
		if (UserSettings)
		{
			UEnhancedPlayerMappableKeyProfile* CurrentProfile = UserSettings->GetActiveKeyProfile();
			if (CurrentProfile)
			{
				const TMap<FName, FKeyMappingRow>& KeyMappings = CurrentProfile->GetPlayerMappingRows();
				for (const auto& [RowName, MappingRow] : KeyMappings)
				{
					for (const FPlayerKeyMapping& CurrentKeyMapping : MappingRow.Mappings)
					{
						if (CurrentKeyMapping.GetAssociatedInputAction() == InputAction)
						{
							FText DisplayName = CurrentKeyMapping.GetDisplayName();
							if (!DisplayName.IsEmpty()) return DisplayName;
							break;
						}
					}
				}
			}
		}
	}

	FString ActionDisplayName = InputAction->GetName();

	return FText::FromString(ActionDisplayName);
}

bool UMCore_EnhancedInputDisplay::IsActionRemappable(APlayerController* PlayerController, UInputAction* InputAction)
{
	return false;
}

bool UMCore_EnhancedInputDisplay::RemapActionKey(APlayerController* PlayerController, UInputAction* InputAction,
	FKey NewKey, FText& OutError)
{
	return false;
}

bool UMCore_EnhancedInputDisplay::ResetActionToDefault(APlayerController* PlayerController, UInputAction* InputAction,
	FText& OutError)
{
	return false;
}
