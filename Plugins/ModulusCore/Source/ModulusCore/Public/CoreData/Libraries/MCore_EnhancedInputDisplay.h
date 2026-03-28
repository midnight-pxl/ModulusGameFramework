// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_EnhancedInputDisplay.h
 *
 * Blueprint function library for querying and remapping Enhanced Input key bindings.
 * Wraps UE's EnhancedInputUserSettings and PlayerMappableKeyProfile systems.
 */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonInputTypeEnum.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Styling/SlateBrush.h"
#include "MCore_EnhancedInputDisplay.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UEnhancedPlayerMappableKeyProfile;
class APlayerController;
class UInputAction;
class ULocalPlayer;

/**
 * Enhanced Input display and rebinding helpers.
 * Query current key bindings, retrieve display names, and remap keys through
 * Enhanced Input's native PlayerMappableKeySettings system.
 */
UCLASS()
class MODULUSCORE_API UMCore_EnhancedInputDisplay : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// ============================================================================
	// KEY QUERIES
	// ============================================================================

	/** Returns the first bound key for the given input action, or Invalid if unbound. */
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static FKey GetCurrentKeyForAction(APlayerController* PlayerController, UInputAction* InputAction);

	/** Returns all bound keys for the given input action. */
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static TArray<FKey> GetAllKeysForAction(APlayerController* PlayerController, UInputAction* InputAction);

	// ============================================================================
	// ACTION METADATA
	// ============================================================================

	/** Returns the display name from PlayerMappableKeySettings, or empty if not configured. */
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static FText GetActionDisplayName(APlayerController* PlayerController, UInputAction* InputAction);

	/** Returns the display category from PlayerMappableKeySettings, or empty if not configured. */
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static FText GetActionDisplayCategory(APlayerController* PlayerController, UInputAction* InputAction);

	/** Returns true if the input action has PlayerMappableKeySettings and is marked remappable. */
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static bool IsActionRemappable(APlayerController* PlayerController, UInputAction* InputAction);

	/** Returns the mapping name from PlayerMappableKeySettings for Data Table display. */
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static FName GetActionMappingName(APlayerController* PlayerController, UInputAction* InputAction);

	// ============================================================================
	// REBINDING OPERATIONS
	// ============================================================================

	/**
	 * Remap an input action to a new key. Validates that the action is remappable
	 * and handles slot-based binding through the active key profile.
	 */
	UFUNCTION(BlueprintCallable, Category = "Enhanced Input Display")
	static bool RemapActionKey(APlayerController* PlayerController,
							  UInputAction* InputAction,
							  FKey NewKey,
							  FText& OutError);

	/** Resets a specific mapping slot for an action to its default binding. */
	UFUNCTION(BlueprintCallable, Category = "Enhanced Input Display")
	static bool ResetActionToDefault(APlayerController* PlayerController,
									UInputAction* InputAction,
									UPARAM(DisplayName = "Error Message") FText& OutError);

	// ============================================================================
	// REMAPPABLE ACTION QUERIES
	// ============================================================================

	/** Returns all player-mappable key mappings from the active key profile. */
	UFUNCTION(BlueprintCallable, Category = "Modulus|Input Display")
	static TArray<FPlayerKeyMapping> GetAllRemappableActions(APlayerController* PlayerController);

	/** Returns player-mappable key mappings filtered to a specific device type. */
	UFUNCTION(BlueprintCallable, Category = "Modulus|Input Display")
	static TArray<FPlayerKeyMapping> GetRemappableActionsForDevice(
		APlayerController* PlayerController, ECommonInputType DeviceType);

	// ============================================================================
	// ICON RESOLUTION
	// ============================================================================

	/** Resolve an FKey to its platform-appropriate icon brush using the current input device. */
	UFUNCTION(BlueprintCallable, Category = "Modulus|Input Display")
	static bool GetIconBrushForKey(const ULocalPlayer* LocalPlayer, FKey Key, FSlateBrush& OutBrush);

	/** Resolve an FKey to its icon brush for a specific device type (for dual-column display). */
	UFUNCTION(BlueprintCallable, Category = "Modulus|Input Display")
	static bool GetIconBrushForKeyByDeviceType(const ULocalPlayer* LocalPlayer, FKey Key,
		ECommonInputType InputType, FSlateBrush& OutBrush);

	// ============================================================================
	// SLOT-BASED OPERATIONS
	// ============================================================================

	/** Returns the bound key for a specific action, slot, and device type. */
	UFUNCTION(BlueprintPure, Category = "Modulus|Input Display")
	static FKey GetBoundKeyForSlot(APlayerController* PlayerController, UInputAction* InputAction,
		EPlayerMappableKeySlot Slot, bool bGamepad);

	/** Remap an action to a new key in a specific slot. Device derived from NewKey. */
	UFUNCTION(BlueprintCallable, Category = "Modulus|Input Display")
	static bool RemapActionKeyForSlot(APlayerController* PlayerController, UInputAction* InputAction,
		FKey NewKey, EPlayerMappableKeySlot Slot, FText& OutError);

	/** Reset an action's binding to default for a specific slot and device type. */
	UFUNCTION(BlueprintCallable, Category = "Modulus|Input Display")
	static bool ResetActionToDefaultForSlot(APlayerController* PlayerController, UInputAction* InputAction,
		EPlayerMappableKeySlot Slot, bool bGamepad, FText& OutError);

private:
	/* Resolve Enhanced Input subsystem from PlayerController with validation */
	static UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem(APlayerController* PlayerController);

	/* Resolve the active key profile (Subsystem -> UserSettings -> Profile) */
	static UEnhancedPlayerMappableKeyProfile* GetActiveKeyProfile(APlayerController* PlayerController);
};
