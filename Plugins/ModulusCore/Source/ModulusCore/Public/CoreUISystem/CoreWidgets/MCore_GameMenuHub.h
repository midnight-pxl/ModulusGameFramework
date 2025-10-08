// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MCore_GameMenuHub.generated.h"

class UCommonActivatableWidgetStack;
class UMCore_ButtonBase;
class UPanelWidget;
struct FMCore_MenuTab;

  /**
   * In-Game Menu Hub - Tabbed interface for plugin menu pages
   * 
   * Architecture:
   * - Lives on MCore_GameMenuLayer when active
   * - Dynamically builds tab bar from registered tabs
   * - Displays active tab's page widget in stack
   * 
   * Usage (Open Menu):
   *   UISubsystem->GetPrimaryGameLayout()->MCore_GameMenuLayer->AddWidget(UMCore_GameMenuHub::StaticClass());
   * 
   * Plugin Registration:
   *   UISubsystem->RegisterMenuTab(FMCore_MenuTab{Label, Icon, PageWidget, Priority});
   */
  UCLASS(Abstract, BlueprintType, Blueprintable)
  class MODULUSCORE_API UMCore_GameMenuHub : public UCommonActivatableWidget
  {
      GENERATED_BODY()

  public:
      UMCore_GameMenuHub(const FObjectInitializer& ObjectInitializer);

  protected:
      virtual void NativeOnInitialized() override;
      virtual void NativeOnActivated() override;

      /** Rebuild tab bar from currently registered tabs */
      UFUNCTION(BlueprintCallable, Category = "Menu Hub")
      void RebuildTabBar();

      /** Handle tab button click (switches active page) */
      UFUNCTION(BlueprintCallable, Category = "Menu Hub")
      void OnTabClicked(UCommonButtonBase* Button, int32 TabIndex);

      /** Switch to specific tab by index */
      UFUNCTION(BlueprintCallable, Category = "Menu Hub")
      void SwitchToTab(int32 TabIndex);

      // === BLUEPRINT BINDINGS ===

      /** Widget stack for displaying active tab's page */
      UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
      TObjectPtr<UCommonActivatableWidgetStack> PageStack;

      /** Container widget for tab buttons (HorizontalBox, etc.) */
      UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
      TObjectPtr<UPanelWidget> TabButtonContainer;

      /** Blueprint class to use for tab buttons */
      UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu Hub")
      TSubclassOf<UMCore_ButtonBase> TabButtonClass;

  private:
      /** Cached registered tabs (refreshed on activation) */
      UPROPERTY(Transient)
      TArray<FMCore_MenuTab> CachedTabs;

      /** Currently active tab index */
      int32 ActiveTabIndex{-1};
  };

