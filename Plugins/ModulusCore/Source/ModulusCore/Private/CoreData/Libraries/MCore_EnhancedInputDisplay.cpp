// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreData/Libraries/MCore_EnhancedInputDisplay.h"

#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInput/Public/UserSettings/EnhancedInputUserSettings.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "CoreData/Logging/LogModulusSettings.h"

// =============================================================================
// Private Helpers
// =============================================================================

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

UEnhancedPlayerMappableKeyProfile* UMCore_EnhancedInputDisplay::GetActiveKeyProfile(
	APlayerController* PlayerController)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem(PlayerController);
	if (!Subsystem) return nullptr;
	
	UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
	if (!UserSettings) return nullptr;
	
	return UserSettings->GetActiveKeyProfile();
}

// =============================================================================
// Key Queries
// =============================================================================

FKey UMCore_EnhancedInputDisplay::GetCurrentKeyForAction(APlayerController* PlayerController,
	UInputAction* InputAction)
{
	if (!InputAction) return EKeys::Invalid;
	
	UEnhancedPlayerMappableKeyProfile* Profile = GetActiveKeyProfile(PlayerController);
	if (!Profile) return EKeys::Invalid;
	
	const FKeyMappingRow* KeyRow = Profile->FindKeyMappingRow(InputAction->GetFName());
	if (!KeyRow) return EKeys::Invalid;
	
	for (const FPlayerKeyMapping& KeyMapping : KeyRow->Mappings)
	{
		return KeyMapping.GetCurrentKey();
	}
	
	return EKeys::Invalid;
}

TArray<FKey> UMCore_EnhancedInputDisplay::GetAllKeysForAction(APlayerController* PlayerController,
	UInputAction* InputAction)
{
	TArray<FKey> Keys;
	if (!InputAction) return Keys;
	
	UEnhancedPlayerMappableKeyProfile* KeyProfile = GetActiveKeyProfile(PlayerController);
	if (!KeyProfile) return Keys;
	
	const FKeyMappingRow* KeyRow = KeyProfile->FindKeyMappingRow(InputAction->GetFName());
	if (!KeyRow) return Keys;
	
	for (const FPlayerKeyMapping& KeyMapping : KeyRow->Mappings)
	{
		Keys.Add(KeyMapping.GetCurrentKey());
	}

	return Keys;
}

// =============================================================================
// Action Metadata
// =============================================================================

FText UMCore_EnhancedInputDisplay::GetActionDisplayName(APlayerController* PlayerController,
	UInputAction* InputAction)
{
	if (!InputAction) return FText::FromString(TEXT("Invalid InputAction"));

	UEnhancedPlayerMappableKeyProfile* KeyProfile = GetActiveKeyProfile(PlayerController);
	if (!KeyProfile) return FText::FromString(InputAction->GetName());
	
	const FKeyMappingRow* KeyRow = KeyProfile->FindKeyMappingRow(InputAction->GetFName());
	if (!KeyRow) return FText::FromString(InputAction->GetName());
	
	for (const FPlayerKeyMapping& KeyMapping : KeyRow->Mappings)
	{
		FText DisplayName = KeyMapping.GetDisplayName();
		if (!DisplayName.IsEmpty()) return DisplayName;
	}
	
	return FText::FromString(InputAction->GetName());
}

FText UMCore_EnhancedInputDisplay::GetActionDisplayCategory(APlayerController* PlayerController,
	UInputAction* InputAction)
{
	if (!InputAction) return FText::FromString(TEXT("Invalid InputAction"));
	
	UEnhancedPlayerMappableKeyProfile* KeyProfile = GetActiveKeyProfile(PlayerController);
	if (!KeyProfile) return FText::FromString(TEXT("General"));
	
	const FKeyMappingRow* KeyRow = KeyProfile->FindKeyMappingRow(InputAction->GetFName());
	
	return FText::FromString(TEXT("General"));
}

FName UMCore_EnhancedInputDisplay::GetActionMappingName(APlayerController* PlayerController,
	UInputAction* InputAction)
{
	if (!InputAction) return NAME_None;

	UEnhancedPlayerMappableKeyProfile* KeyProfile = GetActiveKeyProfile(PlayerController);
	if (!KeyProfile) return NAME_None;
	
	const FKeyMappingRow* KeyRow = KeyProfile->FindKeyMappingRow(InputAction->GetFName());
	if (!KeyRow) return NAME_None;
	
	/** KeyRow exists in KeyProfile (row key == mapping name */
	return InputAction->GetFName();
}

bool UMCore_EnhancedInputDisplay::IsActionRemappable(APlayerController* PlayerController,
	UInputAction* InputAction)
{
	if (!InputAction) return false;

	UEnhancedPlayerMappableKeyProfile* KeyProfile = GetActiveKeyProfile(PlayerController);
	if (!KeyProfile) return false;

	/** If row exists for action, return true */
	return KeyProfile->FindKeyMappingRow(InputAction->GetFName()) != nullptr;
}

// =============================================================================
// Rebinding Operations
// =============================================================================

bool UMCore_EnhancedInputDisplay::RemapActionKey(APlayerController* PlayerController,
	UInputAction* InputAction,
	FKey NewKey,
	FText& OutError)
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

	/** Confirm action is remappable before attempt */
	FName MappingName = InputAction->GetFName();
	UEnhancedPlayerMappableKeyProfile* KeyProfile = UserSettings->GetActiveKeyProfile();
	if (!KeyProfile || !KeyProfile->FindKeyMappingRow(MappingName))
	{
		OutError = FText::FromString(TEXT("Action not remappable or mapping invalid"));
		return false;
	}
	
	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = MappingName;
	KeyArgs.NewKey = NewKey;
	KeyArgs.Slot = EPlayerMappableKeySlot::First;
	
	FGameplayTagContainer FailureReason;
	UserSettings->MapPlayerKey(KeyArgs, FailureReason);
	
	/** Check if the operation succeeded */
	if (FailureReason.IsEmpty())
	{
		// Success - save the settings
		UserSettings->SaveSettings();
		UE_LOG(LogModulusSettings, Log, TEXT("Successfully remapped action %s to key %s"), 
			   *InputAction->GetName(), *NewKey.ToString());
		return true;
	}

	/** Failed - convert failure reason to readable text */
	OutError = FText::FromString(FailureReason.ToString());
	UE_LOG(LogModulusSettings, Warning, TEXT("Failed to remap action %s to key %s: %s"), 
		   *InputAction->GetName(), *NewKey.ToString(), *FailureReason.ToString());
	return false;
}

bool UMCore_EnhancedInputDisplay::ResetActionToDefault(APlayerController* PlayerController,
	UInputAction* InputAction,
	FText& OutError)
{
	OutError = FText::GetEmpty();

	if (!InputAction)
	{
		OutError = FText::FromString(TEXT("Input Action is null"));
		return false;
	}
	
	UEnhancedPlayerMappableKeyProfile* KeyProfile = GetActiveKeyProfile(PlayerController);
	if (!KeyProfile)
	{
		OutError = FText::FromString(TEXT("No active key profile"));
		return false;
	}

	const FKeyMappingRow* MappingRow = KeyProfile->FindKeyMappingRow(InputAction->GetFName());
	if (!MappingRow)
	{
		OutError = FText::FromString(TEXT("Action not found in key profile"));
		return false;
	}
	
	for (const FPlayerKeyMapping& Mapping : MappingRow->Mappings)
	{
		FKey DefaultKey = Mapping.GetDefaultKey();
		return RemapActionKey(PlayerController, InputAction, DefaultKey, OutError);
	}

	OutError = FText::FromString(TEXT("No mappings found for action"));
	return false;
}
