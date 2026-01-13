// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MCore_PDA_SoundStyle.generated.h"

class USoundBase;

/**
 * UI sound styling
 * Referenced by UMCore_PDA_UITheme_Base
 */
UCLASS(BlueprintType, Blueprintable)
class MODULUSCORE_API UMCore_PDA_SoundStyle : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	/** Button hover/focus sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buttons")
	TObjectPtr<USoundBase> ButtonHoverSound;

	/** Button click/select sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buttons")
	TObjectPtr<USoundBase> ButtonClickSound;

	/** Button disabled/invalid click sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buttons")
	TObjectPtr<USoundBase> ButtonDisabledSound;

	// =========================================================================
	// Settings Sounds
	// =========================================================================
	
	/** Setting value changed sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<USoundBase> SettingChangeSound;

	/** Slider drag sound (looping) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<USoundBase> SliderDragSound;

	/** Dropdown open sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<USoundBase> DropdownOpenSound;

	/** Dropdown close sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<USoundBase> DropdownCloseSound;

	// =========================================================================
	// Menu Sounds
	// =========================================================================
	
	/** Menu open sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu")
	TObjectPtr<USoundBase> MenuOpenSound;

	/** Menu close sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu")
	TObjectPtr<USoundBase> MenuCloseSound;

	/** Tab switch sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu")
	TObjectPtr<USoundBase> TabSwitchSound;

	/** Back/cancel navigation sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu")
	TObjectPtr<USoundBase> BackSound;

	// =========================================================================
	// Feedback Sounds
	// =========================================================================
	
	/** Positive feedback (save success, valid input) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Feedback")
	TObjectPtr<USoundBase> PositiveSound;

	/** Negative feedback (error, invalid input) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Feedback")
	TObjectPtr<USoundBase> NegativeSound;

	/** Warning sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Feedback")
	TObjectPtr<USoundBase> WarningSound;

	// =========================================================================
	// Game Flow Sounds
	// =========================================================================
	
	/** Game start/continue sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow")
	TObjectPtr<USoundBase> GameStartSound;

	/** Quit confirm sound */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow")
	TObjectPtr<USoundBase> QuitSound;
};
