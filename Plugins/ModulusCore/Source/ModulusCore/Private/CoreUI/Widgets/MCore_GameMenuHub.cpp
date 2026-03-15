// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUI/Widgets/MCore_GameMenuHub.h"

#include "GameplayTagContainer.h"
#include "CommonButtonBase.h"
#include "CommonAnimatedSwitcher.h"
#include "CoreUI/MCore_UISubsystem.h"
#include "CoreUI/Widgets/Primitives/MCore_TabbedContainer.h"
#include "CoreData/Logging/LogModulusUI.h"
#include "CoreData/Types/UI/MCore_MenuTabTypes.h"

UMCore_GameMenuHub::UMCore_GameMenuHub(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{
}

void UMCore_GameMenuHub::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (TabbedContainer)
    {
        TabbedContainer->OnTabAdded.AddDynamic(this, &ThisClass::HandleContainerTabAdded);
    }
}

void UMCore_GameMenuHub::RebuildTabBar()
{
    if (!TabbedContainer)
    {
        UE_LOG(LogModulusUI, Error,
            TEXT("GameMenuHub: TabbedContainer not bound - verify BindWidget in Blueprint"));
        return;
    }

    ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
    if (!LocalPlayer)
    {
        UE_LOG(LogModulusUI, Warning, TEXT("GameMenuHub: No owning LocalPlayer"));
        return;
    }

    UMCore_UISubsystem* UISubsystem = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
    if (!UISubsystem)
    {
        UE_LOG(LogModulusUI, Warning, TEXT("GameMenuHub: No UISubsystem found"));
        return;
    }

    const TArray<FMCore_MenuTab>& RegisteredScreens = UISubsystem->GetRegisteredMenuScreens();

    TabbedContainer->ClearAllTabs();

    if (RegisteredScreens.IsEmpty())
    {
        if (EmptyStateWidgetClass)
        {
            UCommonActivatableWidget* EmptyWidget = CreateWidget<UCommonActivatableWidget>(
                GetOwningPlayer(), EmptyStateWidgetClass);

            if (EmptyWidget)
            {
                UCommonAnimatedSwitcher* Switcher = TabbedContainer->GetPageSwitcher();
                if (Switcher)
                {
                    Switcher->AddChild(EmptyWidget);
                    Switcher->SetActiveWidget(EmptyWidget);
                }
                UE_LOG(LogModulusUI, Log, TEXT("GameMenuHub: Showing empty state (no screens registered)"));
            }
        }
        else
        {
            UE_LOG(LogModulusUI, Warning, TEXT("GameMenuHub: No screens registered and EmptyStateWidgetClass not set"));
        }
        return;
    }

    for (int32 Index = 0; Index < RegisteredScreens.Num(); ++Index)
    {
        const FMCore_MenuTab& Tab = RegisteredScreens[Index];
        FName TabNameID = FName(*Tab.TabID.ToString());

        UCommonActivatableWidget* ScreenWidget = CreateWidget<UCommonActivatableWidget>(
            GetOwningPlayer(), Tab.ScreenWidgetClass);

        if (!ScreenWidget)
        {
            UE_LOG(LogModulusUI, Error,
                TEXT("GameMenuHub: Failed to create widget for '%s'"),
                *Tab.TabID.ToString());
            continue;
        }

        TabbedContainer->AddTab(TabNameID, ScreenWidget);
        OnPageCreated(TabNameID, ScreenWidget);
    }

    if (!RegisteredScreens.IsEmpty())
    {
        FName FirstTabID = FName(*RegisteredScreens[0].TabID.ToString());
        TabbedContainer->SelectTab(FirstTabID);

        UE_LOG(LogModulusUI, Log,
            TEXT("GameMenuHub: Rebuilt tab bar with %d tabs"),
            RegisteredScreens.Num());
    }
}

bool UMCore_GameMenuHub::SetTabEnabled(FGameplayTag TabID, bool bEnabled)
{
    if (!TabbedContainer || !TabID.IsValid()) return false;

    FName TabNameID = FName(*TabID.ToString());
    bool bSuccess = TabbedContainer->SetTabEnabled(TabNameID, bEnabled);

    if (bSuccess)
    {
        UE_LOG(LogModulusUI, Log, TEXT("SetTabEnabled: Tab '%s' is %s"),
            *TabID.ToString(),
            bEnabled ? TEXT("ENABLED") : TEXT("DISABLED"));
    }
    else
    {
        UE_LOG(LogModulusUI, Warning, TEXT("SetTabEnabled: Tab '%s' not found"),
            *TabID.ToString());
    }

    return bSuccess;
}

bool UMCore_GameMenuHub::SetTabHidden(FGameplayTag TabID, bool bIsHidden)
{
    if (!TabbedContainer || !TabID.IsValid()) return false;

    FName TabNameID = FName(*TabID.ToString());
    bool bSuccess = TabbedContainer->SetTabHidden(TabNameID, bIsHidden);

    if (bSuccess)
    {
        UE_LOG(LogModulusUI, Log, TEXT("SetTabHidden: Tab '%s' is %s"),
            *TabID.ToString(),
            bIsHidden ? TEXT("HIDDEN") : TEXT("VISIBLE"));
    }
    else
    {
        UE_LOG(LogModulusUI, Warning, TEXT("SetTabHidden: Tab '%s' not found"),
            *TabID.ToString());
    }

    return bSuccess;
}

bool UMCore_GameMenuHub::IsTabEnabled(FGameplayTag TabID) const
{
    if (!TabbedContainer || !TabID.IsValid()) return false;

    return TabbedContainer->IsTabEnabled(FName(*TabID.ToString()));
}

bool UMCore_GameMenuHub::IsTabHidden(FGameplayTag TabID) const
{
    if (!TabbedContainer || !TabID.IsValid()) return false;

    return TabbedContainer->IsTabHidden(FName(*TabID.ToString()));
}

void UMCore_GameMenuHub::HandleContainerTabAdded(FName TabID, UCommonButtonBase* TabButton)
{
    OnTabCreated(TabID, TabButton);
}
