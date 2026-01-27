// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreData/Types/UI/MCore_ThemeTypes.h"
#include "Engine/DeveloperSettings.h"
#include "MCore_CoreSettings.generated.h"

class UMCore_PDA_UITheme_Base;
class UMCore_PrimaryGameLayout;
class UMCore_GameMenuHub;

/**
 * Core settings for the Modulus Game Framework.
 *
 * Accessible via:
 * - Project Settings > Game > Modulus Core
 * - Modulus Hub editor panel
 *
 * Theme Configuration:
 * - Define available themes for your game
 * - Set which theme loads by default
 *
 * Debug Settings (Editor-only):
 * - Event system logging for debugging cross-plugin communication
 * - UI layer visualization for CommonUI debugging
 */
UCLASS(Config=ModulusCore, DefaultConfig, meta=(DisplayName="Modulus Core"))
class MODULUSCORE_API UMCore_CoreSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UMCore_CoreSettings();

	//~ UDeveloperSettings Interface
	virtual FName GetCategoryName() const override { return TEXT("Game"); }
	virtual FName GetSectionName() const override { return TEXT("Modulus Core"); }
#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;
#endif
	//~ End UDeveloperSettings Interface

	/** Get the singleton settings instance */
	static const UMCore_CoreSettings* Get();

	//~ Begin Theme Configuration
	
	/**
	 * Widget class for the PrimaryGameLayout.
	 * This is the root UI widget containing the 4-layer CommonUI stack.
	 * 
	 * Leave empty to use default UMCore_PrimaryGameLayout.
	 * Set to a Blueprint subclass for custom layout structure.
	 */
	UPROPERTY(Config, EditAnywhere, Category="UI", meta=(DisplayName="Primary Game Layout Class"))
	TSoftClassPtr<UMCore_PrimaryGameLayout> PrimaryGameLayoutClass;
	
	/**
	 * Widget class for the GameMenuHub.
	 * This is the tabbed menu container for plugin screens.
	 * 
	 * Leave empty to use default UMCore_GameMenuHub.
	 * Set to a Blueprint subclass for custom hub appearance.
	 */
	UPROPERTY(Config, EditAnywhere, Category="UI", meta=(DisplayName="Menu Hub Class"))
	TSoftClassPtr<UMCore_GameMenuHub> MenuHubClass;
	
	/**
	 * Z-Order for PrimaryGameLayout in viewport.
	 * Higher values render on top of lower values.
	 * Default: 0 (base layer, behind most game UI)
	 */
	UPROPERTY(Config, EditAnywhere, Category="UI", meta=(DisplayName="Layout Z-Order", ClampMin="-100", ClampMax="100"))
	int32 PrimaryGameLayoutZOrder{0};
	
	/**
	 * Theme Configuration
	 */
	
	/**
	 * Available UI themes for this project.
	 * Add theme entries here to make them selectable at runtime.
	 * Each entry has a display name, description, and theme DataAsset.
	 * Order determines display order in theme selection UI.
	 */
	UPROPERTY(Config, EditAnywhere, Category="Theme", meta=(DisplayName="Available Themes"))
	TArray<FMCore_ThemeEntry> AvailableThemes;

	/**
	 * Index into AvailableThemes for the default theme.
	 * This theme loads automatically when UI initializes.
	 */
	UPROPERTY(Config, EditAnywhere, Category="Theme", meta=(DisplayName="Default Theme", ClampMin="0"))
	int32 DefaultThemeIndex{0};
	//~ End Theme Configuration

	//~ Begin Debug Settings (Editor Only)
#if WITH_EDITORONLY_DATA
	/**
	 * Log all event broadcasts and subscriptions to Output Log.
	 * Useful for debugging cross-plugin communication.
	 *
	 * Shows: Event tag, payload data, subscriber count, broadcast scope
	 */
	UPROPERTY(Config, EditAnywhere, Category="Debug", meta=(DisplayName="Enable Event System Logging"))
	bool bEnableEventSystemLogging{false};

	/**
	 * Show debug overlay for UI layer stack.
	 * Displays which widgets are on each CommonUI layer.
	 *
	 * Helps debug: Widget not appearing, wrong layer, input not working
	 */
	UPROPERTY(Config, EditAnywhere, Category="Debug", meta=(DisplayName="Show UI Layer Debug Overlay"))
	bool bShowUILayerDebugOverlay{false};
#endif
	//~ End Debug Settings

	//~ Begin Helper Functions

	/** Returns true if event logging is enabled (always false in shipping builds) */
	UFUNCTION(BlueprintPure, Category="Modulus|Debug")
	bool IsEventLoggingEnabled() const;

	/** Returns true if UI debug overlay is enabled (always false in shipping builds) */
	UFUNCTION(BlueprintPure, Category="Modulus|Debug")
	bool IsUIDebugOverlayEnabled() const;

	/** Get the default theme asset (loads synchronously if needed) */
	UFUNCTION(BlueprintCallable, Category="Modulus|Theme")
	UMCore_PDA_UITheme_Base* GetDefaultTheme() const;

	/**
	 * Get default theme for design-time preview (static, no UISubsystem needed).
	 * Use this in NativePreConstruct() for UMG editor preview.
	 * At runtime, prefer UISubsystem->GetActiveTheme() instead.
	 */
	UFUNCTION(BlueprintPure, Category="Modulus|Theme")
	static UMCore_PDA_UITheme_Base* GetDesignTimeTheme();

	/** Check if a theme index is valid */
	UFUNCTION(BlueprintPure, Category="Modulus|Theme")
	bool IsValidThemeIndex(int32 Index) const;
};
