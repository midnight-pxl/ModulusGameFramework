// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_CoreSettings.h
 *
 * Developer settings for the Modulus Game Framework, accessible via
 * Project Settings > Game > Modulus Core. Configures themes, UI layout,
 * settings collection, and editor-only debug options.
 */

#pragma once

#include "CoreMinimal.h"
#include "CoreData/Types/UI/MCore_ThemeTypes.h"
#include "Engine/DeveloperSettings.h"
#include "MCore_CoreSettings.generated.h"

class UMCore_PDA_UITheme_Base;
class UMCore_PrimaryGameLayout;
class UMCore_GameMenuHub;
class UMCore_DA_SettingsCollection;

/**
 * Developer settings for the Modulus Game Framework (Project Settings > Game > Modulus Core).
 * Configures themes, UI layout class overrides, default settings collection,
 * and editor-only debug options.
 */
UCLASS(Config=ModulusCore, DefaultConfig, meta=(DisplayName="Modulus Core"))
class MODULUSCORE_API UMCore_CoreSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UMCore_CoreSettings();

	// ============================================================================
	// DEVELOPER SETTINGS INTERFACE
	// ============================================================================

	virtual FName GetCategoryName() const override { return TEXT("Game"); }
	virtual FName GetSectionName() const override { return TEXT("Modulus Core"); }
#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;
#endif

	static const UMCore_CoreSettings* Get();

	// ============================================================================
	// UI LAYOUT
	// ============================================================================

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

	/* Z-Order for PrimaryGameLayout in viewport. Higher values render on top. */
	UPROPERTY(Config, EditAnywhere, Category="UI", meta=(DisplayName="Layout Z-Order", ClampMin="-100", ClampMax="100"))
	int32 PrimaryGameLayoutZOrder{0};

	// ============================================================================
	// THEME CONFIGURATION
	// ============================================================================

	/**
	 * Available UI themes for this project.
	 * Each entry has a display name, description, and theme DataAsset.
	 * Order determines display order in theme selection UI.
	 */
	UPROPERTY(Config, EditAnywhere, Category="Theme", meta=(DisplayName="Available Themes"))
	TArray<FMCore_ThemeEntry> AvailableThemes;

	/* Index into AvailableThemes for the default theme. Loads automatically on UI init. */
	UPROPERTY(Config, EditAnywhere, Category="Theme", meta=(DisplayName="Default Theme", ClampMin="0"))
	int32 DefaultThemeIndex{0};

	// ============================================================================
	// SETTINGS COLLECTION
	// ============================================================================

	/**
	 * The default settings collection for this project.
	 * Used by tag-based setting accessors and bulk reset operations
	 * when no explicit collection reference is provided.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	TObjectPtr<UMCore_DA_SettingsCollection> DefaultSettingsCollection;

	// ============================================================================
	// DEBUG (EDITOR ONLY)
	// ============================================================================

#if WITH_EDITORONLY_DATA
	/**
	 * Log all event broadcasts and subscriptions to Output Log.
	 * Shows: Event tag, payload data, subscriber count, broadcast scope.
	 */
	UPROPERTY(Config, EditAnywhere, Category="Debug", meta=(DisplayName="Enable Event System Logging"))
	bool bEnableEventSystemLogging{false};

	/**
	 * Show debug overlay for UI layer stack.
	 * Helps debug: Widget not appearing, wrong layer, input not working.
	 */
	UPROPERTY(Config, EditAnywhere, Category="Debug", meta=(DisplayName="Show UI Layer Debug Overlay"))
	bool bShowUILayerDebugOverlay{false};
#endif

	// ============================================================================
	// HELPERS
	// ============================================================================

	/** Returns true if event system logging is enabled. Always false in shipping builds. */
	UFUNCTION(BlueprintPure, Category="Modulus|Debug")
	bool IsEventLoggingEnabled() const;

	/** Returns true if the UI debug overlay is enabled. Always false in shipping builds. */
	UFUNCTION(BlueprintPure, Category="Modulus|Debug")
	bool IsUIDebugOverlayEnabled() const;

	/** Returns the theme DataAsset at DefaultThemeIndex (loads synchronously). Returns nullptr if index is invalid. */
	UFUNCTION(BlueprintCallable, Category="Modulus|Theme")
	UMCore_PDA_UITheme_Base* GetDefaultTheme() const;

	/**
	 * Returns the default theme for design-time preview (static, no UISubsystem needed).
	 * Use in NativePreConstruct() for UMG editor preview. At runtime, prefer UISubsystem->GetActiveTheme().
	 */
	UFUNCTION(BlueprintPure, Category="Modulus|Theme")
	static UMCore_PDA_UITheme_Base* GetDesignTimeTheme();

	UFUNCTION(BlueprintPure, Category="Modulus|Theme")
	bool IsValidThemeIndex(int32 Index) const;
};
