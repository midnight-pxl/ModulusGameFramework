// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreData/Libraries/MCore_EnhancedInputDisplay.h"

#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInput/Public/UserSettings/EnhancedInputUserSettings.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "CoreData/Logging/LogModulusSettings.h"

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
						}
						break;
					}
				}
			}
		}
	}
	
	return FText::FromString(InputAction->GetName());
}

FText UMCore_EnhancedInputDisplay::GetActionDisplayCategory(APlayerController* PlayerController, UInputAction* InputAction)
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
							FText DisplayCategory = CurrentKeyMapping.GetDisplayCategory();
							if (!DisplayCategory.IsEmpty()) return DisplayCategory;
						}
						break;
					}
				}
			}
		}
	}
	
	return FText::FromString(TEXT("General"));
}

FName UMCore_EnhancedInputDisplay::GetActionMappingName(APlayerController* PlayerController, UInputAction* InputAction)
{
	if (!InputAction) { return NAME_None; }

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
							// The RowName is the mapping name used for remapping
							return RowName;
						}
					}
				}
			}
		}
	}

	return NAME_None;
}

bool UMCore_EnhancedInputDisplay::IsActionRemappable(APlayerController* PlayerController, UInputAction* InputAction)
{
	if (!InputAction) 
	{ 
		return false; 
	}

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
							// If it exists in the player mappable profile, it's remappable
							return true;
						}
					}
				}
			}
		}
	}

	return false; // Not found in player mappable profile = not remappable
}

bool UMCore_EnhancedInputDisplay::RemapActionKey(APlayerController* PlayerController, UInputAction* InputAction,
                                                 FKey NewKey, FText& OutError)
{
	OutError = FText::GetEmpty();

	if (!InputAction)
	{
		OutError = FText::FromString(TEXT("Input Action is null"));
		return false;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem(PlayerController);
	if (!Subsystem)
	{
		OutError = FText::FromString(TEXT("Enhanced Input subsystem not available"));
		return false;
	}

	UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
	if (!UserSettings)
	{
		OutError = FText::FromString(TEXT("User settings not initialized"));
		return false;
	}

	// Get the mapping name for this action
	FName MappingName = GetActionMappingName(PlayerController, InputAction);
	if (MappingName == NAME_None)
	{
		OutError = FText::FromString(TEXT("Action is not remappable or mapping name not found"));
		return false;
	}

	// Perform the remapping
	FMapPlayerKeyArgs Args;
	Args.MappingName = MappingName;
	Args.NewKey = NewKey;
	Args.Slot = EPlayerMappableKeySlot::First;

	FGameplayTagContainer FailureReason;
	UserSettings->MapPlayerKey(Args, FailureReason);

	// Check if the operation succeeded
	if (FailureReason.IsEmpty())
	{
		// Success - save the settings
		UserSettings->SaveSettings();
		UE_LOG(LogModulusSettings, Log, TEXT("Successfully remapped action %s to key %s"), 
			   *InputAction->GetName(), *NewKey.ToString());
		return true;
	}
	else
	{
		// Failed - convert failure reason to readable text
		FString FailureString = FailureReason.ToString();
		OutError = FText::FromString(FailureString);
		UE_LOG(LogModulusSettings, Warning, TEXT("Failed to remap action %s to key %s: %s"), 
			   *InputAction->GetName(), *NewKey.ToString(), *FailureString);
		return false;
	}
}

bool UMCore_EnhancedInputDisplay::ResetActionToDefault(APlayerController* PlayerController, UInputAction* InputAction,
	FText& OutError)
{
	OutError = FText::GetEmpty();

	if (!IsActionRemappable(PlayerController, InputAction))
	{
		UE_LOG(LogModulusSettings, Error, TEXT("ResetActionToDefault: Action is not remappable"));
		return false;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem(PlayerController);
	if (!Subsystem)
	{
		UE_LOG(LogModulusSettings, Error, TEXT("ResetActionToDefault: Enhanced Input subsystem not available"));
		return false;
	}

	UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
	if (!UserSettings)
	{
		UE_LOG(LogModulusSettings, Error, TEXT("ResetActionToDefault: User settings not initialized"));
		return false;
	}

	UEnhancedPlayerMappableKeyProfile* CurrentProfile = UserSettings->GetActiveKeyProfile();
	if (!CurrentProfile)
	{
		UE_LOG(LogModulusSettings, Error, TEXT("ResetActionToDefault: No active key profile"));
		return false;
	}

	// Find the default key for this action
	const TMap<FName, FKeyMappingRow>& MappingRows = CurrentProfile->GetPlayerMappingRows();
	for (const auto& [RowName, MappingRow] : MappingRows)
	{
		for (const FPlayerKeyMapping& Mapping : MappingRow.Mappings)
		{
			if (Mapping.GetAssociatedInputAction() == InputAction)
			{
				FKey DefaultKey = Mapping.GetDefaultKey();
				return RemapActionKey(PlayerController, InputAction, DefaultKey, OutError);
			}
		}
	}

	UE_LOG(LogModulusSettings, Error, TEXT("ResetActionToDefault: Default mapping not found"));
	return false;
}
