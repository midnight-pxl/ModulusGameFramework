// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "CoreData/CoreStructEnums/UIStructsEnums/MCore_MenuTabTypes.h"
#include "MCore_UISubsystem.generated.h"

class UCommonTabListWidgetBase;
class UMCore_PrimaryGameLayout;
class UCommonActivatableWidget;
struct FMCore_MenuTab;

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

	/** Get the primary game layout widget for this local player */
	UFUNCTION(BlueprintPure, Category = "ModulusUI|Layout")
	UMCore_PrimaryGameLayout* GetPrimaryGameLayout() const;

	/**
	 * Register primary game layout (called automatically during layout construction)
	 *
	 * Do not call manually - layout registers itself in NativeOnInitialized().
	 *
	 * @return True if registration succeeded
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusUI|Layout")
	bool RegisterPrimaryGameLayout(UMCore_PrimaryGameLayout* InLayout);

	/**
     * Register a menu screen from a plugin
     *
     * Plugins call this during component BeginPlay to add their screens
     * to the in-game menu hub. Screens appear as tabs, sorted by priority.
     *
     * Example Usage (ModulusVault):
     *   UISubsystem->RegisterMenuScreen(
     *       InventoryScreenClass,
     *       LOCTEXT("Inventory", "Inventory"),
     *       100  // Priority: lower numbers appear first
     *   );
     *
     * Common Priorities:
     * - 100 = Character/Inventory
     * - 200 = Quests/Journal
     * - 300 = Map/World
     * - 400 = Social/Guild
     * - 500 = System/Settings
     *
     * @param ScreenWidgetClass - Widget class for the menu screen
     * @param TabName - Display name for the tab
     * @param Priority - Lower numbers appear first (default 100)
     * @param TabIcon - Optional Icon for the tab
     */
    UFUNCTION(BlueprintCallable, Category = "ModulusUI|MenuHub")
    void RegisterMenuScreen(
        TSubclassOf<UCommonActivatableWidget> ScreenWidgetClass,
        const FText& TabName,
        int32 Priority = 100,
        UTexture2D* TabIcon = nullptr
    );

    /**
     * Check if a screen is already registered
     *
     * Prevents duplicate registration when components re-initialize.
     *
     * @param ScreenWidgetClass - Widget class to check
     * @return True if already registered
     */
    UFUNCTION(BlueprintPure, Category = "ModulusUI|MenuHub")
    bool IsScreenRegistered(TSubclassOf<UCommonActivatableWidget> ScreenWidgetClass) const;

	/**
	 * Get all registered menu tabs (sorted by priority)
	 *
	 * Menu hub widget calls this during NativeOnInitialized to build tabs.
	 * Returns tabs in priority order (lower priority values first).
	 *
	 * @return Array of registered menu tabs
	 */
	UFUNCTION(BlueprintPure, Category = "ModulusUI|MenuHub")
	const TArray<FMCore_MenuTab>& GetRegisteredMenuScreens() const { return RegisteredMenuScreens; }

    /**
     * Get count of registered menu screens
     *
     * Useful for debugging or conditional UI layout.
     *
     * @return Number of registered screens
     */
    UFUNCTION(BlueprintPure, Category = "ModulusUI|MenuHub")
    int32 GetRegisteredScreenCount() const { return RegisteredMenuScreens.Num(); }

private:
	UPROPERTY()
	TSubclassOf<UMCore_PrimaryGameLayout> PrimaryGameLayoutClass;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<UMCore_PrimaryGameLayout> CachedPrimaryGameLayout;

	/** Registered menu screens for this local player */
	UPROPERTY(Transient)
	TArray<FMCore_MenuTab> RegisteredMenuScreens;

	/** Cached reference to menu hub tab list (if created) */
	UPROPERTY(Transient)
	TWeakObjectPtr<UCommonTabListWidgetBase> CachedMenuHub;
};
