// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MCore_EnhancedInputDisplay.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_EnhancedInputDisplay : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Get current key bound to an action for display purposes
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static FKey GetCurrentKeyForAction(APlayerController* PlayerController, UInputAction* InputAction);
    
	// Get all keys bound to an action (for multiple bindings)
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static TArray<FKey> GetAllKeysForAction(APlayerController* PlayerController, UInputAction* InputAction);
    
	// Get display name for showing in UI
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static FText GetActionDisplayName(APlayerController* PlayerController, UInputAction* InputAction);
    
	// Check if action is player-remappable
	UFUNCTION(BlueprintPure, Category = "Enhanced Input Display")
	static bool IsActionRemappable(APlayerController* PlayerController, UInputAction* InputAction);
    
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
	// Helper function to get Enhanced Input subsystem with validation
	static UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem(APlayerController* PlayerController);
};
