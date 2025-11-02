// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreData/CoreStructEnums/SettingsStructsEnums/MCore_SettingsPresets.h"
#include "MCore_GameSettingsLibrary.generated.h"

class UInputAction;
/**
 * Game settings library
 *
 * Convenience wrappers around UGameUserSettings for common settings operations.
 * Integrates with Unreal Engine's built-in graphics/audio/input systems.
 *
 * Blueprint Usage:
 * - Call Set* functions from settings menus
 * - Call ApplyAndSaveAllSettings() when user clicks "Apply" button
 * - Quality values: 0 = Low, 1 = Medium, 2 = High, 3 = Ultra
 *
 * Note: Most settings require ApplyAndSaveAllSettings() to take effect.
 */
UCLASS()
class MODULUSCORE_API UMCore_GameSettingsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

    /** Apply graphics quality preset (affects all scalability settings) */
    UFUNCTION(BlueprintCallable, Category = "Graphics")
    static void ApplyGraphicsPreset(EMCore_GraphicsPreset Preset);

	/** Get current graphics preset based on scalability levels */
	UFUNCTION(BlueprintPure, Category = "Graphics")
	static EMCore_GraphicsPreset GetCurrentGraphicsPreset();

	/**
	 * Set Look Sensitivity X (Horizontal)
	 * 
	 * @param XSensitivity Multiplier (0.1 = very slow, 1.0 = default, 10.0 = very fast)
	 * 
	 * Enhanced Input Scalar modifier:
	 *   1. Get current sensitivity via this function
	 *   2. Apply to InputModifierScalar on your IA_Look action
	 *   3. Modifier automatically multiplies input by sensitivity value
	 */
	UFUNCTION(BlueprintCallable, Category = "InputSettings")
	static void SetLookSensitivityX(float XSensitivity);

	/**
	 * Get Look Sensitivity X (Horizontal)
	 * Returns stored horizontal look sensitivity (default: 1.0)
	 */
	UFUNCTION(BlueprintPure, Category = "InputSettings")
	static float GetLookSensitivityX(float XSensitivity);

	/**
	 * Set Look Sensitivity Y (Vertical)
	 * 
	 * @param YSensitivity Multiplier (0.1 = very slow, 1.0 = default, 10.0 = very fast)
	 */
	UFUNCTION(BlueprintCallable, Category = "InputSettings")
	static void SetLookSensitivityY(float YSensitivity);

	/**
	 * Get Look Sensitivity Y (Vertical)
	 * Returns stored vertical look sensitivity (default: 1.0)
	 */
	UFUNCTION(BlueprintPure, Category = "InputSettings")
	static float GetLookSensitivityY(float YSensitivity);
	
	/**
	 * Set Invert Look Y (Vertical)
	 * 
	 * Standard Y-axis inversion for flight controls or user preference.
	 * Conditionally add/remove InputModifierNegate based on this setting
	 * 
	 * @param bInvert True = inverted (up input looks down), False = normal
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Input")
	static void SetInvertLookY(bool bInvert);
    
	/**
	 * Get Invert Look Y
	 * Returns current Y-axis invert state (default: false)
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus Settings|Input")
	static bool GetInvertLookY();
    
	/**
	 * Set Invert Look X (Horizontal)
	 * 
	 * Less common than Y invert, but useful for some control schemes.
	 * 
	 * @param bInvert True = inverted (right input looks left), False = normal
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus Settings|Input")
	static void SetInvertLookX(bool bInvert);
    
	/**
	 * Get Invert Look X
	 * Returns current X-axis invert state (default: false)
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus Settings|Input")
	static bool GetInvertLookX();

private:
	// Console variable names for look sensitivity storage
	static constexpr const TCHAR* CVAR_LOOK_SENSITIVITY_X = TEXT("Settings.Input.LookSensitivityX");
	static constexpr const TCHAR* CVAR_LOOK_SENSITIVITY_Y = TEXT("Settings.Input.LookSensitivityY");
	static constexpr const TCHAR* CVAR_INVERT_LOOK_X = TEXT("Settings.Input.InvertLookX");
	static constexpr const TCHAR* CVAR_INVERT_LOOK_Y = TEXT("Settings.Input.InvertLookY");
    
	// Graphics preset helpers
	static int32 ConvertPresetToScalabilityLevel(EMCore_GraphicsPreset Preset);
	static EMCore_GraphicsPreset ConvertScalabilityLevelToPreset(int32 Level);
};
