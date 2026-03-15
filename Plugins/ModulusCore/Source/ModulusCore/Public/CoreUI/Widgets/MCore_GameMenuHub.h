// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MCore_GameMenuHub.generated.h"

class UCommonButtonBase;
class UCommonActivatableWidget;
class UMCore_TabbedContainer;
struct FGameplayTag;

/**
 * In-Game Menu Hub - Tabbed interface for plugin menu pages
 *
 * Architecture:
 * - Lives on MCore_GameMenuLayer when active
 * - Composes UMCore_TabbedContainer for tab/page management
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
    //~ UUserWidget Interface
    virtual void NativeOnInitialized() override;
    //~ End UUserWidget Interface

    /** Tabbed container primitive — handles tab list + page switcher */
    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    TObjectPtr<UMCore_TabbedContainer> TabbedContainer;

    UPROPERTY(EditDefaultsOnly, Category = "Menu Hub")
    TSubclassOf<UCommonActivatableWidget> EmptyStateWidgetClass;

private:
    /** Forwarding handler for TabbedContainer's OnTabAdded delegate */
    UFUNCTION()
    void HandleContainerTabAdded(FName TabID, UCommonButtonBase* TabButton);
};
