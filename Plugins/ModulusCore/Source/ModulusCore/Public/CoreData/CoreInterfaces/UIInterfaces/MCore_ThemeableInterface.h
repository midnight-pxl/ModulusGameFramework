// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MCore_ThemeableInterface.generated.h"

class UMCore_DA_UITheme_Base;

UINTERFACE(MinimalAPI, Blueprintable)
class UMCore_ThemeableInterface : public UInterface
{
	GENERATED_BODY()
};

class MODULUSCORE_API IMCore_ThemeableInterface
{
	GENERATED_BODY()

public:
	/**
	 * Called when the active theme changes.
	 * 
	 * Apply theme properties to your widget's visual elements here.
	 * This is called automatically by RefreshTheme() and when the
	 * global theme changes in Developer Settings.
	 * 
	 * @param NewTheme The newly active theme data asset
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Modulus|Theme")
	void OnThemeChanged(const UMCore_DA_UITheme_Base* NewTheme);
	virtual void OnThemeChanged_Implementation(const UMCore_DA_UITheme_Base* NewTheme) {}
    
	/**
	 * Request theme refresh from Developer Settings.
	 * Call this in NativeOnInitialized() to apply the initial theme.
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus|Theme")
	void RefreshTheme();
	
};
