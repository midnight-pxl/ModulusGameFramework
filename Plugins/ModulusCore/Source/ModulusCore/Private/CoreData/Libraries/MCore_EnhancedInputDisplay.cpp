// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreData/Libraries/MCore_EnhancedInputDisplay.h"

#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInput/Public/UserSettings/EnhancedInputUserSettings.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "CoreData/Logging/LogModulusSettings.h"
#include "CommonInputSubsystem.h"
#include "CommonInputBaseTypes.h"
#include "CoreUI/MCore_UISubsystem.h"
#include "CoreUI/Settings/MCore_PlayerSettingsSave.h"

// ============================================================================
// PRIVATE HELPERS
// ============================================================================

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

// ============================================================================
// KEY QUERIES
// ============================================================================

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

// ============================================================================
// ACTION METADATA
// ============================================================================

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
	if (!KeyRow) return FText::FromString(TEXT("General"));

	for (const FPlayerKeyMapping& KeyMapping : KeyRow->Mappings)
	{
		FText DisplayCat = KeyMapping.GetDisplayCategory();
		if (!DisplayCat.IsEmpty()) return DisplayCat;
	}

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

	// Row key matches mapping name
	return InputAction->GetFName();
}

bool UMCore_EnhancedInputDisplay::IsActionRemappable(APlayerController* PlayerController,
	UInputAction* InputAction)
{
	if (!InputAction) return false;

	UEnhancedPlayerMappableKeyProfile* KeyProfile = GetActiveKeyProfile(PlayerController);
	if (!KeyProfile) return false;

	return KeyProfile->FindKeyMappingRow(InputAction->GetFName()) != nullptr;
}

// ============================================================================
// REBINDING OPERATIONS
// ============================================================================

/**
 * @param InputAction   DataTable row reference to the action. Must have PlayerMappableKeySettings.
 * @param NewKey        Key to bind. Invalid key is rejected.
 * @param MappingSlot   Slot index (0 = primary, 1 = secondary, etc.).
 * @return              True if rebind succeeded, false if action is not remappable or key is invalid.
 */
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

	// Confirm action is remappable before attempt
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

	if (FailureReason.IsEmpty())
	{
		UserSettings->SaveSettings();
		UE_LOG(LogModulusSettings, Log, TEXT("Successfully remapped action %s to key %s"),
			   *InputAction->GetName(), *NewKey.ToString());
		return true;
	}

	OutError = FText::FromString(FailureReason.ToString());
	UE_LOG(LogModulusSettings, Warning, TEXT("Failed to remap action %s to key %s: %s"),
		   *InputAction->GetName(), *NewKey.ToString(), *FailureReason.ToString());
	return false;
}

/**
 * @param MappingSlot  Slot index to reset (0 = primary, 1 = secondary).
 */
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

// ============================================================================
// REMAPPABLE ACTION QUERIES
// ============================================================================

TArray<FPlayerKeyMapping> UMCore_EnhancedInputDisplay::GetAllRemappableActions(
	APlayerController* PlayerController)
{
	TArray<FPlayerKeyMapping> Result;

	UEnhancedPlayerMappableKeyProfile* Profile = GetActiveKeyProfile(PlayerController);
	if (!Profile)
	{
		UE_LOG(LogModulusSettings, Warning, TEXT("GetAllRemappableActions: No active key profile"));
		return Result;
	}

	const TMap<FName, FKeyMappingRow>& Rows = Profile->GetPlayerMappingRows();
	for (const TPair<FName, FKeyMappingRow>& Pair : Rows)
	{
		for (const FPlayerKeyMapping& Mapping : Pair.Value.Mappings)
		{
			Result.Add(Mapping);
		}
	}

	return Result;
}

TArray<FPlayerKeyMapping> UMCore_EnhancedInputDisplay::GetRemappableActionsForDevice(
	APlayerController* PlayerController, ECommonInputType DeviceType)
{
	TArray<FPlayerKeyMapping> Result;

	UEnhancedPlayerMappableKeyProfile* Profile = GetActiveKeyProfile(PlayerController);
	if (!Profile)
	{
		UE_LOG(LogModulusSettings, Warning, TEXT("GetRemappableActionsForDevice: No active key profile"));
		return Result;
	}

	const TMap<FName, FKeyMappingRow>& Rows = Profile->GetPlayerMappingRows();
	for (const TPair<FName, FKeyMappingRow>& Pair : Rows)
	{
		for (const FPlayerKeyMapping& Mapping : Pair.Value.Mappings)
		{
			// Use current key for device check; fall back to default if current is invalid
			FKey KeyToCheck = Mapping.GetCurrentKey();
			if (!KeyToCheck.IsValid())
			{
				KeyToCheck = Mapping.GetDefaultKey();
			}

			bool bInclude = false;
			switch (DeviceType)
			{
			case ECommonInputType::Gamepad:
				bInclude = KeyToCheck.IsGamepadKey();
				break;
			case ECommonInputType::MouseAndKeyboard:
				bInclude = !KeyToCheck.IsGamepadKey() && !KeyToCheck.IsTouch();
				break;
			case ECommonInputType::Touch:
				bInclude = KeyToCheck.IsTouch();
				break;
			default:
				break;
			}

			if (bInclude)
			{
				Result.Add(Mapping);
			}
		}
	}

	return Result;
}

// ============================================================================
// ICON RESOLUTION
// ============================================================================

bool UMCore_EnhancedInputDisplay::GetIconBrushForKey(const ULocalPlayer* LocalPlayer,
	FKey Key, FSlateBrush& OutBrush)
{
	if (!LocalPlayer || !Key.IsValid()) { return false; }

	UCommonInputSubsystem* CIS = UCommonInputSubsystem::Get(LocalPlayer);
	if (!CIS)
	{
		UE_LOG(LogModulusSettings, Warning, TEXT("GetIconBrushForKey: CommonInputSubsystem unavailable"));
		return false;
	}

	UCommonInputPlatformSettings* PlatformSettings = UCommonInputPlatformSettings::Get();
	if (!PlatformSettings) { return false; }

	return PlatformSettings->TryGetInputBrush(
		OutBrush, Key, CIS->GetCurrentInputType(), CIS->GetCurrentGamepadName());
}

bool UMCore_EnhancedInputDisplay::GetIconBrushForKeyByDeviceType(const ULocalPlayer* LocalPlayer,
	FKey Key, ECommonInputType InputType, FSlateBrush& OutBrush)
{
	if (!LocalPlayer || !Key.IsValid()) { return false; }

	UCommonInputPlatformSettings* PlatformSettings = UCommonInputPlatformSettings::Get();
	if (!PlatformSettings) { return false; }

	// Gamepad icons need the active gamepad name to pick the right controller data
	FName GamepadName = NAME_None;
	if (InputType == ECommonInputType::Gamepad)
	{
		UCommonInputSubsystem* CIS = UCommonInputSubsystem::Get(LocalPlayer);
		if (CIS)
		{
			GamepadName = CIS->GetCurrentGamepadName();
		}
	}

	return PlatformSettings->TryGetInputBrush(OutBrush, Key, InputType, GamepadName);
}

// ============================================================================
// GAMEPAD CONFIG HELPERS
// ============================================================================

TArray<FName> UMCore_EnhancedInputDisplay::GetAvailableGamepadConfigs(
	const UObject* WorldContextObject)
{
	TArray<FName> Result;

	UCommonInputPlatformSettings* PlatformSettings = UCommonInputPlatformSettings::Get();
	if (!PlatformSettings) { return Result; }

	const TArray<TSoftClassPtr<UCommonInputBaseControllerData>> ControllerData =
		PlatformSettings->GetControllerData();

	for (const TSoftClassPtr<UCommonInputBaseControllerData>& SoftClass : ControllerData)
	{
		const UClass* LoadedClass = SoftClass.LoadSynchronous();
		if (!LoadedClass) { continue; }

		const UCommonInputBaseControllerData* CDO =
			LoadedClass->GetDefaultObject<UCommonInputBaseControllerData>();
		if (!CDO || CDO->InputType != ECommonInputType::Gamepad) { continue; }

		Result.AddUnique(CDO->GamepadName);
	}

	return Result;
}

/**
 * @param LocalPlayer  Used to access PlayerSettingsSave and CommonInputSubsystem.
 */
FName UMCore_EnhancedInputDisplay::GetEffectiveGamepadName(
	const UObject* WorldContextObject, const ULocalPlayer* LocalPlayer)
{
	if (!LocalPlayer) { return NAME_None; }

	// Read the player's override index
	int32 OverrideIndex = 0;
	UMCore_UISubsystem* UISub = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
	if (UISub)
	{
		if (UMCore_PlayerSettingsSave* Save = UISub->GetPlayerSettings())
		{
			OverrideIndex = Save->GamepadIconSetIndex;
		}
	}

	// 0 = Auto-Detect: use whatever CommonInput reports
	if (OverrideIndex == 0)
	{
		UCommonInputSubsystem* CIS = UCommonInputSubsystem::Get(LocalPlayer);
		return CIS ? CIS->GetCurrentGamepadName() : NAME_None;
	}

	// Non-zero: look up the config at (index - 1)
	TArray<FName> Configs = GetAvailableGamepadConfigs(WorldContextObject);
	const int32 AdjustedIndex = OverrideIndex - 1;

	if (Configs.IsValidIndex(AdjustedIndex))
	{
		return Configs[AdjustedIndex];
	}

	// Out of range: fall back to auto-detect
	UCommonInputSubsystem* CIS = UCommonInputSubsystem::Get(LocalPlayer);
	return CIS ? CIS->GetCurrentGamepadName() : NAME_None;
}

// ============================================================================
// SLOT-BASED OPERATIONS
// ============================================================================

FKey UMCore_EnhancedInputDisplay::GetBoundKeyForSlot(APlayerController* PlayerController,
	UInputAction* InputAction, EPlayerMappableKeySlot Slot, bool bGamepad)
{
	if (!InputAction) { return EKeys::Invalid; }

	UEnhancedPlayerMappableKeyProfile* Profile = GetActiveKeyProfile(PlayerController);
	if (!Profile) { return EKeys::Invalid; }

	const FKeyMappingRow* Row = Profile->FindKeyMappingRow(InputAction->GetFName());
	if (!Row) { return EKeys::Invalid; }

	for (const FPlayerKeyMapping& Mapping : Row->Mappings)
	{
		if (Mapping.GetSlot() == Slot && Mapping.GetCurrentKey().IsGamepadKey() == bGamepad)
		{
			return Mapping.GetCurrentKey();
		}
	}

	return EKeys::Invalid;
}

bool UMCore_EnhancedInputDisplay::RemapActionKeyForSlot(APlayerController* PlayerController,
	UInputAction* InputAction, FKey NewKey, EPlayerMappableKeySlot Slot, FText& OutError)
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
	KeyArgs.Slot = Slot;
	KeyArgs.bCreateMatchingSlotIfNeeded = true;

	FGameplayTagContainer FailureReason;
	UserSettings->MapPlayerKey(KeyArgs, FailureReason);

	if (FailureReason.IsEmpty())
	{
		UserSettings->SaveSettings();
		UE_LOG(LogModulusSettings, Log, TEXT("Remapped action %s (slot %d) to key %s"),
			*InputAction->GetName(), static_cast<int32>(Slot), *NewKey.ToString());
		return true;
	}

	OutError = FText::FromString(FailureReason.ToString());
	UE_LOG(LogModulusSettings, Warning, TEXT("Failed to remap action %s (slot %d) to key %s: %s"),
		*InputAction->GetName(), static_cast<int32>(Slot), *NewKey.ToString(),
		*FailureReason.ToString());
	return false;
}

bool UMCore_EnhancedInputDisplay::ResetActionToDefaultForSlot(APlayerController* PlayerController,
	UInputAction* InputAction, EPlayerMappableKeySlot Slot, bool bGamepad, FText& OutError)
{
	OutError = FText::GetEmpty();

	if (!InputAction)
	{
		OutError = FText::FromString(TEXT("Input Action is null"));
		return false;
	}

	UEnhancedPlayerMappableKeyProfile* Profile = GetActiveKeyProfile(PlayerController);
	if (!Profile)
	{
		OutError = FText::FromString(TEXT("No active key profile"));
		return false;
	}

	const FKeyMappingRow* Row = Profile->FindKeyMappingRow(InputAction->GetFName());
	if (!Row)
	{
		OutError = FText::FromString(TEXT("Action not found in key profile"));
		return false;
	}

	// Find the mapping matching the requested slot and device type
	for (const FPlayerKeyMapping& Mapping : Row->Mappings)
	{
		if (Mapping.GetSlot() == Slot && Mapping.GetDefaultKey().IsGamepadKey() == bGamepad)
		{
			return RemapActionKeyForSlot(PlayerController, InputAction,
				Mapping.GetDefaultKey(), Slot, OutError);
		}
	}

	OutError = FText::FromString(TEXT("No matching mapping found for slot and device type"));
	return false;
}
