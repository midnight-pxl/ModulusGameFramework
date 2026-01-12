// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "CoreData/CoreStructEnums/UIStructsEnums/MCore_MenuTabTypes.h"
#include "MCore_UISubsystem.generated.h"

class UCommonActivatableWidgetStack;
class UMCore_DA_UITheme_Base;
class UTexture2D;
class UCommonTabListWidgetBase;
class UMCore_GameMenuHub;
class UMCore_PrimaryGameLayout;
class UCommonActivatableWidget;
struct FGameplayTag;

/**
 * For accessing the primary game layout (CommonUI Widget Stacks)
 * and registering with ModulusCore's In-Game Menu Hub
 *
 * Provides access to the PrimaryGameLayout widget and its layer stacks.
 * One instance per LocalPlayer (supports split-screen).
 *
 * Common Uses:
 * - Push menus to MenuLayer (settings, pause menu)
 * - Push HUD to GameLayer (health bar, minimap)
 * - Push modals to ModalLayer (confirmation dialogs)
 * - Register plugin menu screens (inventory, quests, etc.)
 *
 * Blueprint Usage:
 * 1. Get UI Subsystem from LocalPlayer
 * 2. GetPrimaryGameLayout() to access layer stacks
 * 3. Use CommonUI AddWidget/RemoveWidget on desired layer
 *
 * Network: Client-only
 * - Zero server overhead (LocalPlayerSubsystem)
 * - Each client manages their own UI independently
 */
UCLASS()
class MODULUSCORE_API UMCore_UISubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	/**~ USubsystem Interface */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	/**~ End USubsystem Interface */

	/** Get the primary game layout widget for this local player */
	UFUNCTION(BlueprintPure, Category = "UI|Layout")
	UMCore_PrimaryGameLayout* GetPrimaryGameLayout() const;

	/**
	 * Returns whether a valid PrimaryGameLayout is registered.
	 */
	UFUNCTION(BlueprintPure, Category = "UI|Layout")
	bool HasPrimaryGameLayout() const { return CachedPrimaryGameLayout.IsValid(); }
	
	/**
	 * Register primary game layout (called automatically during layout construction)
	 *
	 * Do not call manually - layout registers itself in NativeOnInitialized().
	 *
	 * @return True if registration succeeded
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|Layout")
	bool RegisterPrimaryGameLayout(UMCore_PrimaryGameLayout* InLayout);

	/**
	 * Unregister primary game layout (called by AMCore_HUD_Base during EndPlay).
	 * 
	 * Clears the cached reference. Does NOT destroy the widget (HUD owns it).
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|Layout")
	void UnregisterPrimaryGameLayout();
	
	/** 
	 * Get or create the menu hub widget
	 * Caller is responsible for pushing to appropriate stack
	 * 
	 * @return MenuHub widget (created on first call, cached for reuse)
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|MenuHub")
	UMCore_GameMenuHub* GetOrCreateMenuHub();

	/** Menu screen registration */
	UFUNCTION(BlueprintCallable, Category = "UI|MenuHub")
	void RegisterMenuScreen(
		FGameplayTag TabID,
		TSubclassOf<UCommonActivatableWidget> ScreenWidgetClass,
		int32 Priority = 100,
		UTexture2D* TabIcon = nullptr);

	/**
	 * Unregister a menu screen tab.
	 * 
	 * @param TabID Gameplay tag of the tab to remove
	 * @return True if tab was found and removed
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|MenuHub")
	bool UnregisterMenuScreen(FGameplayTag TabID);
	
	UFUNCTION(BlueprintPure, Category = "UI|MenuHub")
	const TArray<FMCore_MenuTab>& GetRegisteredMenuScreens() const
	{
		return RegisteredMenuScreens;
	}

	/**
	 * Force rebuild of MenuHub tab bar.
	 * Called automatically when screens registered/unregistered while hub is active.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusUI|MenuHub")
	void RebuildMenuHubTabBar();
	
	/**
	 * Get active UI theme from Dev Settings
	 */
	 UFUNCTION(BlueprintCallable, Category = "UI|Theme")
	virtual UMCore_DA_UITheme_Base* GetActiveTheme() const;

protected:
	/** Widget class for MenuHub (loaded from settings or defaults) */
	UPROPERTY()
	TSubclassOf<UMCore_GameMenuHub> MenuHubClass;

private:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<UMCore_PrimaryGameLayout> CachedPrimaryGameLayout;

	/** Cached reference to menu hub tab list (if created) */
	UPROPERTY(Transient)
	TWeakObjectPtr<UMCore_GameMenuHub> CachedMenuHub;
	
	/** Registered menu screens for this local player */
	UPROPERTY(Transient)
	TArray<FMCore_MenuTab> RegisteredMenuScreens;
	
	/**
	 * Load widget classes from configuration
	 * 
	 * Current: Hard-coded asset paths (temporary)
	 * Future: Load from UMCore_UISettings (project settings)
	 * 
	 * Called during Initialize() before CreatePrimaryGameLayout()
	 */
	void LoadWidgetClasses();
};
