// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MCore_GameMenuHub.generated.h"

class UCommonButtonBase;
class UCommonAnimatedSwitcher;
class UCommonTabListWidgetBase;
struct FGameplayTag;

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
     */
    UFUNCTION(BlueprintCallable, Category = "Menu Hub")
    void RebuildTabBar();
    
    UFUNCTION(BlueprintCallable, Category = "Menu Hub", meta = (Keywords = "Toggle Lock Tab Button"))
    bool SetTabEnabled(FGameplayTag TabID, bool bEnabled);
    
    UFUNCTION(BlueprintCallable, Category = "Menu Hub", meta = (Keywords = "Toggle Tab Visibility"))
    bool SetTabHidden(FGameplayTag TabID, bool bIsHidden);
    
    UFUNCTION(BlueprintPure, Category = "Menu Hub", meta = (Keywords = "Is Tab Enabled"))
    bool IsTabEnabled(FGameplayTag TabID) const;
    
    UFUNCTION(BlueprintPure, Category = "Menu Hub", meta = (Keywords = "Is Tab Hidden"))
    bool IsTabHidden(FGameplayTag TabID) const;

    UFUNCTION(BlueprintNativeEvent, Category = "Menu Hub")
    void OnTabCreated(FName TabID, UCommonButtonBase* TabButton);
    void OnTabCreated_Implementation(FName TabID, UCommonButtonBase* TabButton)
    {}

    UFUNCTION(BlueprintNativeEvent, Category = "Menu Hub")
    void OnPageCreated(FName TabID, UCommonActivatableWidget* PageWidget);
    void OnPageCreated_Implementation(FName TabID, UCommonActivatableWidget* PageWidget)
    {}


protected:
    /** Handles button creation, clicks, visual states, gamepad nav */
    UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
    TObjectPtr<UCommonTabListWidgetBase> TabList;

    /** Widget switcher for displaying active tab's page */
    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    TObjectPtr<UCommonAnimatedSwitcher> PageSwitcher;

    /** Tab button class UMCore_ButtonBase for text and icon integration */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu Hub")
    TSubclassOf<UCommonButtonBase> TabButtonClass;

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