// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUISystem/CoreWidgets/MCore_GameMenuHub.h"
#include "CoreUISystem/MCore_UISubsystem.h"
#include "CoreData/CoreStructEnums/UIStructsEnums/MCore_MenuTabTypes.h"
#include "CoreData/CoreLogging/LogModulusUI.h"
#include "CoreUISystem/CoreWidgets/WidgetPrimitives/MCore_ButtonBase.h"
#include "Components/PanelWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UMCore_GameMenuHub::UMCore_GameMenuHub(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{
}

void UMCore_GameMenuHub::NativeOnInitialized()
{
  Super::NativeOnInitialized();

  // Validation
  if (!PageStack)
  {
      UE_LOG(LogModulusUI, Error, TEXT("GameMenuHub: PageStack widget binding missing!"));
  }

  if (!TabButtonContainer)
  {
      UE_LOG(LogModulusUI, Error, TEXT("GameMenuHub: TabButtonContainer widget binding missing!"));
  }

  if (!TabButtonClass)
  {
      UE_LOG(LogModulusUI, Warning, TEXT("GameMenuHub: TabButtonClass not set - tabs will not display"));
  }
}

void UMCore_GameMenuHub::NativeOnActivated()
{
  Super::NativeOnActivated();

  // Rebuild tab bar with current registrations
  RebuildTabBar();

  // Auto-activate first tab if available
  if (CachedTabs.Num() > 0)
  {
      SwitchToTab(0);
  }
  else
  {
      UE_LOG(LogModulusUI, Warning, TEXT("GameMenuHub: No menu tabs registered - menu hub is empty"));
  }
}

void UMCore_GameMenuHub::RebuildTabBar()
{
  if (!TabButtonContainer)
  {
      return;
  }

  // Get UISubsystem
  ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
  if (!LocalPlayer)
  {
      UE_LOG(LogModulusUI, Error, TEXT("GameMenuHub: No owning LocalPlayer - cannot retrieve tabs"));
      return;
  }

  UMCore_UISubsystem* UISubsystem = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
  if (!UISubsystem)
  {
      UE_LOG(LogModulusUI, Error, TEXT("GameMenuHub: UISubsystem not found"));
      return;
  }

  // Refresh tab data
  CachedTabs = UISubsystem->GetRegisteredMenuTabs();

  // Clear existing buttons
  TabButtonContainer->ClearChildren();

  // Create button for each tab
  for (int32 i = 0; i < CachedTabs.Num(); ++i)
  {
      const FMCore_MenuTab& Tab = CachedTabs[i];

      if (!TabButtonClass)
      {
          UE_LOG(LogModulusUI, Warning, TEXT("GameMenuHub: TabButtonClass not set - skipping tab creation"));
          continue;
      }

      UMCore_ButtonBase* TabButton = Cast<UMCore_ButtonBase>(TabButtonClass);
      if (TabButton)
      {
          TabButton->SetButtonText(Tab.TabLabel);

          // Set icon if provided
          if (Tab.TabIcon)
          {
              TabButton->SetButtonIcon(Tab.TabIcon);
          }
      }

      UE_LOG(LogModulusUI, Verbose, TEXT("GameMenuHub: Created tab button for '%s' (Index: %d)"),
          *Tab.TabLabel.ToString(), i);
  }
}

void UMCore_GameMenuHub::OnTabClicked(UCommonButtonBase* Button, int32 TabIndex)
{
  SwitchToTab(TabIndex);
}

void UMCore_GameMenuHub::SwitchToTab(int32 TabIndex)
{
  if (!PageStack)
  {
      UE_LOG(LogModulusUI, Error, TEXT("GameMenuHub: Cannot switch tabs - PageStack is null"));
      return;
  }

  if (!CachedTabs.IsValidIndex(TabIndex))
  {
      UE_LOG(LogModulusUI, Warning, TEXT("GameMenuHub: Invalid tab index %d (only %d tabs registered)"),
          TabIndex, CachedTabs.Num());
      return;
  }

  const FMCore_MenuTab& Tab = CachedTabs[TabIndex];

  if (!Tab.PageWidget)
  {
      UE_LOG(LogModulusUI, Error, TEXT("GameMenuHub: Tab '%s' has null PageWidget"),
          *Tab.TabLabel.ToString());
      return;
  }

  // Clear current page and push new one
  PageStack->ClearWidgets();
  PageStack->AddWidget(Tab.PageWidget);

  ActiveTabIndex = TabIndex;

  UE_LOG(LogModulusUI, Log, TEXT("GameMenuHub: Switched to tab '%s' (Index: %d)"),
      *Tab.TabLabel.ToString(), TabIndex);
}