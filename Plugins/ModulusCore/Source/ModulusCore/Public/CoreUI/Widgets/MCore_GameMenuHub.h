// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_GameMenuHub.h
 *
 * In-game menu hub providing a tabbed interface for plugin menu pages.
 * Composes UMCore_TabbedContainer and rebuilds from UISubsystem's registered screens.
 */

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MCore_GameMenuHub.generated.h"

class UCommonButtonBase;
class UCommonActivatableWidget;
class UMCore_TabbedContainer;
struct FGameplayTag;

/**
 * In-game menu hub - tabbed interface for plugin menu pages.
 *
 * Key Features:
 * - Lives on MCore_GameMenuLayer when active
 * - Composes UMCore_TabbedContainer for tab/page management
 * - Rebuilds tab bar from UISubsystem's registered menu screens
 * - Per-tab enable/disable and show/hide state control
 *
 * Blueprint Usage:
 * - Open: UISubsystem->GetOrCreateMenuHub() adds to GameMenuLayer
 * - Register: UISubsystem->RegisterMenuScreen(ScreenClass, CategoryTag, Priority)
 * - Override OnTabCreated/OnPageCreated for per-tab customization
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class MODULUSCORE_API UMCore_GameMenuHub : public UCommonActivatableWidget
{
    GENERATED_BODY()

public:
    UMCore_GameMenuHub(const FObjectInitializer& ObjectInitializer);

    // ============================================================================
    // PUBLIC API
    // ============================================================================

    /**
     * Rebuild tab bar from currently registered screens.
     * Called by GetOrCreateMenuHub() on creation and RegisterMenuScreen() on dynamic registration.
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

    // ============================================================================
    // BLUEPRINT EXTENSION POINTS
    // ============================================================================

    UFUNCTION(BlueprintNativeEvent, Category = "Menu Hub")
    void OnTabCreated(FName TabID, UCommonButtonBase* TabButton);
    void OnTabCreated_Implementation(FName TabID, UCommonButtonBase* TabButton)
    {}

    UFUNCTION(BlueprintNativeEvent, Category = "Menu Hub")
    void OnPageCreated(FName TabID, UCommonActivatableWidget* PageWidget);
    void OnPageCreated_Implementation(FName TabID, UCommonActivatableWidget* PageWidget)
    {}

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    TObjectPtr<UMCore_TabbedContainer> TabbedContainer;

    UPROPERTY(EditDefaultsOnly, Category = "Menu Hub")
    TSubclassOf<UCommonActivatableWidget> EmptyStateWidgetClass;

private:
    UFUNCTION()
    void HandleContainerTabAdded(FName TabID, UCommonButtonBase* TabButton);
};
