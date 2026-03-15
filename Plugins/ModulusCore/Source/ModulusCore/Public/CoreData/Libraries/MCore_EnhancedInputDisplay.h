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
#include "MCore_EnhancedInputDisplay.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UEnhancedPlayerMappableKeyProfile;
class APlayerController;
class UInputAction;

/**
 * Enhanced Input display and rebinding helpers.
 *
 * Key Features:
 * - Query current key bindings for any input action
 * - Retrieve display names and categories from PlayerMappableKeySettings
 * - Remap and reset key bindings through Enhanced Input's native system
 *
 * Blueprint Usage:
 * - GetCurrentKeyForAction to display the bound key in UI
 * - RemapActionKey to apply a new binding from a key capture widget
 * - ResetActionToDefault to restore original bindings
 */
UCLASS()
class MODULUSCORE_API UMCore_EnhancedInputDisplay : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// ============================================================================
	// KEY QUERIES
	// ============================================================================

	/**
	 * Get the first/primary key currently bound to an input action.
	 *
	 * @param PlayerController The player whose bindings to query
	 * @param InputAction The action to look up
	 * @return The current primary key, or EKeys::Invalid if not found
	 */
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static FKey GetCurrentKeyForAction(APlayerController* PlayerController, UInputAction* InputAction);

	/**
	 * Get all keys bound to an action (for multiple bindings).
	 *
	 * @param PlayerController The player whose bindings to query
	 * @param InputAction The action to look up
	 * @return Array of all bound keys
	 */
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static TArray<FKey> GetAllKeysForAction(APlayerController* PlayerController, UInputAction* InputAction);

	// ============================================================================
	// ACTION METADATA
	// ============================================================================

	/**
	 * Get display name from PlayerMappableKeySettings.
	 *
	 * @param PlayerController The player whose bindings to query
	 * @param InputAction The action to look up
	 * @return The display name, or the action's object name as fallback
	 */
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static FText GetActionDisplayName(APlayerController* PlayerController, UInputAction* InputAction);

	/**
	 * Get display category from PlayerMappableKeySettings.
	 *
	 * @param PlayerController The player whose bindings to query
	 * @param InputAction The action to look up
	 * @return The display category, or "General" as fallback
	 */
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static FText GetActionDisplayCategory(APlayerController* PlayerController, UInputAction* InputAction);

	/**
	 * Check if action is player-remappable.
	 *
	 * @param PlayerController The player whose bindings to query
	 * @param InputAction The action to check
	 * @return True if the action has a mapping row in the active key profile
	 */
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static bool IsActionRemappable(APlayerController* PlayerController, UInputAction* InputAction);

	/**
	 * Get unique mapping name from PlayerMappableKeySettings.
	 *
	 * @param PlayerController The player whose bindings to query
	 * @param InputAction The action to look up
	 * @return The mapping FName, or NAME_None if not found
	 */
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static FName GetActionMappingName(APlayerController* PlayerController, UInputAction* InputAction);

	// ============================================================================
	// REBINDING OPERATIONS
	// ============================================================================

	/**
	 * Remap action through Enhanced Input's native system.
	 *
	 * @param PlayerController The player whose bindings to modify
	 * @param InputAction The action to remap
	 * @param NewKey The new key to bind
	 * @param OutError Error message on failure
	 * @return True on success
	 */
	UFUNCTION(BlueprintCallable, Category = "Enhanced Input Display")
	static bool RemapActionKey(APlayerController* PlayerController,
							  UInputAction* InputAction,
							  FKey NewKey,
							  FText& OutError);

	/**
	 * Reset an input action to its default key binding(s).
	 *
	 * @param PlayerController The player whose bindings to modify
	 * @param InputAction The action to reset
	 * @param OutError Error message on failure
	 * @return True on success
	 */
	UFUNCTION(BlueprintCallable, Category = "Enhanced Input Display")
	static bool ResetActionToDefault(APlayerController* PlayerController,
									UInputAction* InputAction,
									UPARAM(DisplayName = "Error Message") FText& OutError);

private:
	/* Resolve Enhanced Input subsystem from PlayerController with validation */
	static UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem(APlayerController* PlayerController);

	/* Resolve the active key profile (Subsystem -> UserSettings -> Profile) */
	static UEnhancedPlayerMappableKeyProfile* GetActiveKeyProfile(APlayerController* PlayerController);
};
