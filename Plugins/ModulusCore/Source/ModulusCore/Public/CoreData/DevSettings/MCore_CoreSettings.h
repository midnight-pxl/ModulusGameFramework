// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_CoreSettings.h
 *
 * Developer settings for the Modulus Game Framework, accessible via
 * Project Settings > Game > Modulus Core. Configures themes, UI layout,
 * settings collections, and editor-only debug options.
 */

#pragma once

#include "CoreMinimal.h"
#include "CoreData/Types/UI/MCore_ThemeTypes.h"
#include "CoreData/Types/UI/MCore_MenuTabTypes.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "CoreData/Types/Settings/MCore_DA_SettingsCollection.h"
#include "MCore_CoreSettings.generated.h"

class UMCore_PDA_UITheme_Base;
class UMCore_PrimaryGameLayout;
class UMCore_GameMenuHub;
class UMCore_SettingsWidget_Slider;
class UMCore_SettingsWidget_Switcher;
class UMCore_ConfirmationDialog;
class UMCore_KeyBindingPanel_Base;

/**
 * Developer settings for the Modulus Game Framework (Project Settings > Game > Modulus Core).
 * Configures themes, UI layout class overrides, settings collections,
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

	/* Z-Order for PrimaryGameLayout in viewport. Higher values render on top. */
	UPROPERTY(Config, EditAnywhere, Category="UI", meta=(DisplayName="Layout Z-Order", ClampMin="-100", ClampMax="100"))
	int32 PrimaryGameLayoutZOrder{0};

	// ============================================================================
	// MENU HUB
	// ============================================================================

	/**
	 * Widget class for the GameMenuHub.
	 * This is the tabbed menu container for plugin screens.
	 *
	 * Leave empty to use default UMCore_GameMenuHub.
	 * Set to a Blueprint subclass for custom hub appearance.
	 */
	UPROPERTY(Config, EditAnywhere, Category="Menu Hub", meta=(DisplayName="Menu Hub Class"))
	TSoftClassPtr<UMCore_GameMenuHub> MenuHubClass;

	/**
	 * Menu tabs registered on UISubsystem initialization.
	 * Configure all default tabs here, including tabs from other Modulus plugins.
	 * Additional tabs can be added at runtime via RegisterMenuScreen().
	 */
	UPROPERTY(Config, EditAnywhere, Category="Menu Hub", meta=(DisplayName="Default Menu Tabs"))
	TArray<FMCore_MenuTab> DefaultMenuTabs;

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
	// SETTINGS COLLECTIONS
	// ============================================================================

	/**
	 * Settings collections for this project. All collections aggregate into a unified
	 * settings inventory. Categories with the same GameplayTag merge across collections.
	 * Settings sort by SortOrder within merged categories.
	 */
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category="Settings",
		meta=(DisplayName="Settings Collections"))
	TArray<TSoftObjectPtr<UMCore_DA_SettingsCollection>> SettingsCollections;
	
	/** Widget class used to render Slider-type settings. */
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	TSubclassOf<UMCore_SettingsWidget_Slider> SettingsSliderWidgetClass;

	/** Widget class used to render Switcher-type settings (Toggle and Dropdown). */
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	TSubclassOf<UMCore_SettingsWidget_Switcher> SettingsSwitcherWidgetClass;

	/** Widget class for inline key binding panel content in the Settings Panel. */
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	TSubclassOf<UMCore_KeyBindingPanel_Base> KeyBindingPanelClass;

	/** Show secondary binding columns in the key binding panel. */
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	bool bShowSecondaryBindings = false;

	/** Dialog class used for destructive action confirmations (Reset All, etc). */
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	TSubclassOf<UMCore_ConfirmationDialog> ConfirmationDialogClass;

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
	
	// ============================================================================
	// SETTINGS AGGREGATION HELPERS
	// ============================================================================

	/** Returns all loaded settings collections. Resolves soft references on first call, caches results. */
	UFUNCTION(BlueprintPure, Category = "Modulus|Settings")
	const TArray<UMCore_DA_SettingsCollection*>& GetAllSettingsCollections() const;

	/** Searches all collections for a setting definition matching the tag. Returns first match. */
	UFUNCTION(BlueprintPure, Category = "Modulus|Settings")
	UMCore_DA_SettingDefinition* FindSettingDefinitionByTag(const FGameplayTag& SettingTag) const;

	/** Returns all settings across all collections for a category, sorted by SortOrder. */
	UFUNCTION(BlueprintPure, Category = "Modulus|Settings")
	TArray<UMCore_DA_SettingDefinition*> GetSettingsForCategory(const FGameplayTag& CategoryTag) const;

	/** Returns all unique category tags across all collections, sorted by minimum SortOrder per category. */
	UFUNCTION(BlueprintPure, Category = "Modulus|Settings")
	TArray<FGameplayTag> GetAllSettingsCategories() const;

	/** Searches all collections for a category display name. Returns tag leaf segment as fallback. */
	FText GetCategoryDisplayName(const FGameplayTag& CategoryTag) const;

	/** Returns true if at least one collection is assigned and loadable. */
	UFUNCTION(BlueprintPure, Category = "Modulus|Settings")
	bool HasValidSettingsCollections() const;

	/** Clears the resolved collection cache. Next GetAllSettingsCollections() call will re-resolve. */
	void InvalidateCollectionCache();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// ============================================================================
	// THEME HELPERS
	// ============================================================================

	/**
	 * Returns the default theme for design-time preview (static, no UISubsystem needed).
	 * Use in NativePreConstruct() for UMG editor preview. At runtime, prefer UISubsystem->GetActiveTheme().
	 */
	UFUNCTION(BlueprintPure, Category="Modulus|Theme")
	static UMCore_PDA_UITheme_Base* GetDesignTimeTheme();

	UFUNCTION(BlueprintPure, Category="Modulus|Theme")
	bool IsValidThemeIndex(int32 Index) const;

private:
	mutable TArray<TObjectPtr<UMCore_DA_SettingsCollection>> ResolvedCollections;
	mutable bool bCollectionsCacheValid{false};
};
