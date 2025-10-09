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
}

void UMCore_GameMenuHub::OnTabClicked(UCommonButtonBase* Button, int32 TabIndex)
{
  SwitchToTab(TabIndex);
}

void UMCore_GameMenuHub::SwitchToTab(int32 TabIndex)
{
}