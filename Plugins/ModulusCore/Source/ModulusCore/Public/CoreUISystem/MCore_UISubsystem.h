// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "CoreData/CoreStructEnums/UIStructsEnums/MCore_MenuTabTypes.h"
#include "MCore_UISubsystem.generated.h"

class UMCore_PrimaryGameLayout;
struct FMCore_MenuTab;

/**
 * UI subsystem for accessing the primary game layout
 *
 * Provides access to the PrimaryGameLayout widget and its layer stacks.
 * One instance per LocalPlayer (supports split-screen).
 *
 * Common Uses:
 * - Push menus to MenuLayer (settings, pause menu)
 * - Push HUD to GameLayer (health bar, minimap)
 * - Push modals to ModalLayer (confirmation dialogs)
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

	/** Get the primary game layout widget for this local player */
	UFUNCTION(BlueprintPure, Category = "UI Layout")
	UMCore_PrimaryGameLayout* GetPrimaryGameLayout() const;

	/**
	 * Register primary game layout (called automatically during layout construction)
	 *
	 * Do not call manually - layout registers itself in NativeOnInitialized().
	 *
	 * @return True if registration succeeded
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Layout")
	bool RegisterPrimaryGameLayout(UMCore_PrimaryGameLayout* InLayout);

	/**
	 * Register menu tab for in-game menu hub
	 * 
	 * Plugins call this to add tabs to the game menu.
	 * Tabs are sorted by priority (lower = left, higher = right).
	 * 
	 * @param Tab - Tab configuration (label, icon, page widget, priority)
	 * @return Registration handle (store to unregister later)
	 */
	UFUNCTION(BlueprintCallable, Category = "Menu Registration")
	FGuid RegisterMenuTab(const FMCore_MenuTab& Tab);
	
	/**
	 * Unregister menu tab by page widget class
	 * 
	 * @param PageWidget - The page widget class to remove
	 * @return True if tab was found and removed
	 */
	UFUNCTION(BlueprintCallable, Category = "Menu Registration")
	bool UnregisterMenuTab(TSubclassOf<UCommonActivatableWidget> PageWidget);

	/**
	 * Get all registered menu tabs (sorted by priority)
	 * 
	 * Menu hub calls this to rebuild the tab bar.
	 * 
	 * @return Array of registered tabs
	 */
	UFUNCTION(BlueprintPure, Category = "Menu Registration")
	TArray<FMCore_MenuTab> GetRegisteredMenuTabs() const;


private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UMCore_PrimaryGameLayout> CachedPrimaryGameLayout;

	/** Registered menu tabs (sorted by priority) */
	UPROPERTY(Transient)
	TArray<FMCore_MenuTab> RegisteredMenuTabs;

};
