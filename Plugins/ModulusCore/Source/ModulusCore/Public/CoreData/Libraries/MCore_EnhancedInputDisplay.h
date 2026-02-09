// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MCore_EnhancedInputDisplay.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UEnhancedPlayerMappableKeyProfile;
class APlayerController;
class UInputAction;

/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_EnhancedInputDisplay : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Get the first/primary key currently bound to an input action
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static FKey GetCurrentKeyForAction(APlayerController* PlayerController, UInputAction* InputAction);
    
	// Get all keys bound to an action (for multiple bindings)
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static TArray<FKey> GetAllKeysForAction(APlayerController* PlayerController, UInputAction* InputAction);
    
	// Get display name from PlayerMappableKeySettings->DisplayName
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static FText GetActionDisplayName(APlayerController* PlayerController, UInputAction* InputAction);

	// Get display category from PlayerMappableKeySettings->DisplayCategory
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static FText GetActionDisplayCategory(APlayerController* PlayerController, UInputAction* InputAction);
    
	// Check if action is player-remappable
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static bool IsActionRemappable(APlayerController* PlayerController, UInputAction* InputAction);

	// Get unique mapping name from PlayerMappableKeySettings->Name
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static FName GetActionMappingName(APlayerController* PlayerController, UInputAction* InputAction);
    
	// Remap action through Enhanced Input's native system -- returns true on success
	UFUNCTION(BlueprintCallable, Category = "Enhanced Input Display")
	static bool RemapActionKey(APlayerController* PlayerController, 
							  UInputAction* InputAction, 
							  FKey NewKey, 
							  FText& OutError);
	// Reset an input action to its default key binding(s) -- returns true on success
	UFUNCTION(BlueprintCallable, Category = "Enhanced Input Display")
	static bool ResetActionToDefault(APlayerController* PlayerController,
									UInputAction* InputAction,
									UPARAM(DisplayName = "Error Message") FText& OutError);

private:
	/** Resolve Enhanced Input subsystem from PlayerController with validation */
	static UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem(APlayerController* PlayerController);

	/** Resolve the active key profile (Subsystem → UserSettings → Profile) */
	static UEnhancedPlayerMappableKeyProfile* GetActiveKeyProfile(APlayerController* PlayerController);
};
