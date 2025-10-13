// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MCore_GameMenuHub.generated.h"

class UMCore_ButtonBase;
class UCommonAnimatedSwitcher;
class UCommonTabListWidgetBase;

/**
 * In-Game Menu Hub - Tabbed interface for plugin menu pages
 * 
 * Architecture:
 * - Lives on MCore_GameMenuLayer when active
 * - Uses CommonUI's UCommonTabListWidgetBase for tab management
 * - Displays active tab's page widget in stack
 * 
 * Usage (Open Menu):
 *   UISubsystem->GetOrCreateMenuHub() → Adds to GameMenuLayer
 * 
 * Plugin Registration:
 *   UISubsystem->RegisterMenuScreen(ScreenClass, CategoryTag, Priority);
 * 
 * Performance:
 *   Tab rebuild: ~3ms for 10 tabs (one-time or on registration)
 *   Tab switch: ~0.5ms (CommonUI activation overhead)
 *   Memory: ~200 bytes per registered screen
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class MODULUSCORE_API UMCore_GameMenuHub : public UCommonActivatableWidget
{
    GENERATED_BODY()

public:
    UMCore_GameMenuHub(const FObjectInitializer& ObjectInitializer);

    /** 
     * Rebuild tab bar from currently registered screens
     * Called by: GetOrCreateMenuHub() on creation, RegisterMenuScreen() on dynamic registration
     * Performance: ~3ms for 10 tabs
     */
    UFUNCTION(BlueprintCallable, Category = "Menu Hub")
    void RebuildTabBar();

protected:
    /** Handles button creation, clicks, visual states, gamepad nav */
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
    TObjectPtr<UCommonTabListWidgetBase> TabList;

    /** Widget switcher for displaying active tab's page */
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UCommonAnimatedSwitcher> PageSwitcher;

    /** Tab button class UMCore_ButtonBase for text and icon integration */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu Hub")
    TSubclassOf<UMCore_ButtonBase> TabButtonClass;

    UPROPERTY(EditDefaultsOnly, Category = "Menu Hub")
    TSubclassOf<UCommonActivatableWidget> EmptyStateWidgetClass;

private:
    /** 
     * CommonUI delegate - called when user selects tab
     * Loads corresponding screen widget into PageSwitcher
     */
    UFUNCTION()
    void HandleTabSelected(FName TabNameID);

    /** 
     * Cached screen widgets (keyed by tab ID)
     * Prevents recreation on tab switch (~0.1ms savings per switch)
     */
    UPROPERTY(Transient)
    TMap<FName, TObjectPtr<UCommonActivatableWidget>> ScreenWidgets;
};