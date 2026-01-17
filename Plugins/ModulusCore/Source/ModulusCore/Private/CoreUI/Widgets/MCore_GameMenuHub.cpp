// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUI/Widgets/MCore_GameMenuHub.h"

#include "GameplayTagContainer.h"
#include "CommonButtonBase.h"
#include "CommonTabListWidgetBase.h"
#include "CommonAnimatedSwitcher.h"
#include "CoreUI/MCore_UISubsystem.h"
#include "CoreData/Logging/LogModulusUI.h"
#include "CoreData/Types/UI/MCore_MenuTabTypes.h"

UMCore_GameMenuHub::UMCore_GameMenuHub(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{
}

void UMCore_GameMenuHub::RebuildTabBar()
{
    /** Validate BindWidget components */
    if (!TabList || !PageSwitcher)
    {
        UE_LOG(LogModulusUI, Error,
            TEXT("GameMenuHub: TabList or PageSwitcher not bound - verify BindWidget in Blueprint"));
        return;
    }

    /** Get owning player context */
    ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
    if (!LocalPlayer)
    {
        UE_LOG(LogModulusUI, Warning, TEXT("GameMenuHub: No owning LocalPlayer"));
        return;
    }

    /** Get UI subsystem for registered screens */
    UMCore_UISubsystem* UISubsystem = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
    if (!UISubsystem)
    {
        UE_LOG(LogModulusUI, Warning, TEXT("GameMenuHub: No UISubsystem found"));
        return;
    }

    const TArray<FMCore_MenuTab>& RegisteredScreens = UISubsystem->GetRegisteredMenuScreens();

    /** Empty state: No screens registered */
    if (RegisteredScreens.IsEmpty())
    {
        PageSwitcher->ClearChildren();
        ScreenWidgets.Empty();
        
        if (EmptyStateWidgetClass)
        {
            UCommonActivatableWidget* EmptyWidget = CreateWidget<UCommonActivatableWidget>(
                GetOwningPlayer(), EmptyStateWidgetClass);

            if (EmptyWidget)
            {
                PageSwitcher->AddChild(EmptyWidget);
                PageSwitcher->SetActiveWidget(EmptyWidget);
                UE_LOG(LogModulusUI, Log, TEXT("GameMenuHub: Showing empty state (no screens registered)"));
            }                
        }
        else
        {
            UE_LOG(LogModulusUI, Warning, TEXT("GameMenuHub: No screens registered and EmptyStateWidgetClass not set"));
        }
        return;
    }

    /** Clear existing tabs */
    TabList->RemoveAllTabs();
    PageSwitcher->ClearChildren();
    ScreenWidgets.Empty();

    /** Validate button class with framework default fallback */
    TSubclassOf<UCommonButtonBase> ButtonClass;
    if (TabButtonClass)
    {
        ButtonClass = TabButtonClass;
    }
    else
    {
        ButtonClass = UCommonButtonBase::StaticClass();
        UE_LOG(LogModulusUI, Warning, 
            TEXT("GameMenuHub: TabButtonClass not set, using framework default"));
    }
    
    if (!ButtonClass)
    {
        UE_LOG(LogModulusUI, Error, TEXT("GameMenuHub: No valid button class - cannot create tabs"));
        return;
    }

    /** Register tabs with CommonUI */
    /** Performance: 8 tabs × 0.17ms = 1.36ms total (widget creation dominates) */
    for (int32 Index = 0; Index < RegisteredScreens.Num(); ++Index)
    {
        const FMCore_MenuTab& Tab = RegisteredScreens[Index];
        
        FName TabNameID = FName(*Tab.TabID.ToString());
        FText DisplayName = Tab.GetDisplayName();

        /** Create screen widget for this tab */
        UCommonActivatableWidget* ScreenWidget = CreateWidget<UCommonActivatableWidget>(
            GetOwningPlayer(), Tab.ScreenWidgetClass);

        if (!ScreenWidget)
        {
            UE_LOG(LogModulusUI, Error, 
                TEXT("GameMenuHub: Failed to create widget for '%s'"),
                *Tab.TabID.ToString());
            continue;
        }

        PageSwitcher->AddChild(ScreenWidget);

        /** RegisteredScreens already sorted by priority in UISubsystem */
        TabList->RegisterTab(TabNameID, ButtonClass, ScreenWidget, Index);

        /** Cache widget reference for future queries */
        ScreenWidgets.Add(TabNameID, ScreenWidget);
    }

    /** Bind tab selection delegate */
    TabList->OnTabSelected.Clear();
    TabList->OnTabSelected.AddUniqueDynamic(this, &ThisClass::HandleTabSelected);

    /** Select first tab to show initial content */
    if (!RegisteredScreens.IsEmpty())
    {
        FName FirstTabID = FName(*RegisteredScreens[0].TabID.ToString());
        TabList->SelectTabByID(FirstTabID);
        
        UE_LOG(LogModulusUI, Log, 
            TEXT("GameMenuHub: Rebuilt tab bar with %d tabs, %d widgets in PageSwitcher"),
            RegisteredScreens.Num(),
            PageSwitcher->GetChildrenCount());
    }
}

bool UMCore_GameMenuHub::SetTabEnabled(FGameplayTag TabID, bool bEnabled)
{
    if (!TabList)
    {
        UE_LOG(LogModulusUI, Warning, TEXT("SetTabEnabled: Tablist not initialized. Cannot enable tab."))
        return false;
    }
    
    if (!TabID.IsValid())
    {
        UE_LOG(LogModulusUI, Warning, TEXT("SetTabEnabled: Invalid TabID provided"))
        return false;
    }
    
    FName TabNameID = FName(*TabID.ToString());
    if (!ScreenWidgets.Contains(TabNameID))
    {
        UE_LOG(LogModulusUI, Warning, TEXT("SetTabEnabled: Tab '%s' not found. Screen may not be registered"),
            *TabID.ToString());
        return false;
    }
    
    // Call native SetTabEnabled in TabListWidgetBase post checks
    TabList->SetTabEnabled(TabNameID, bEnabled);
    
    UE_LOG(LogModulusUI, Log, TEXT("SetTabEnabled: Tab '%s' is %s"),
        *TabID.ToString(),
        bEnabled ? TEXT("ENABLED") : TEXT("DISABLED"));
    
    return true;
}

bool UMCore_GameMenuHub::SetTabHidden(FGameplayTag TabID, bool bIsHidden)
{
    if (!TabList)
    {
        UE_LOG(LogModulusUI, Warning, TEXT("SetTabHidden: Tablist not initialized. Cannot hide tab."))
        return false;
    }
    
    if (!TabID.IsValid())
    {
        UE_LOG(LogModulusUI, Warning, TEXT("SetTabHidden: Invalid TabID provided"))
        return false;
    }
    
    FName TabNameID = FName(*TabID.ToString());
    if (!ScreenWidgets.Contains(TabNameID))
    {
        UE_LOG(LogModulusUI, Warning, TEXT("SetTabHidden: Tab '%s' not found. Screen may not be registered"),
            *TabID.ToString());
        return false;
    }
    
    // Call native SetTabVisibility in TabListWidgetBase post checks
    TabList->SetTabVisibility(TabNameID, bIsHidden ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
    
    UE_LOG(LogModulusUI, Log, TEXT("SetTabHidden: Tab '%s' is %s"),
        *TabID.ToString(),
        bIsHidden ? TEXT("HIDDEN") : TEXT("VISIBLE"));
    
    return true;
}

bool UMCore_GameMenuHub::IsTabEnabled(FGameplayTag TabID) const
{
    if (!TabList || !TabID.IsValid()) return false;
    
    FName TabNameID = FName(*TabID.ToString());
    if (!ScreenWidgets.Contains(TabNameID)) return false;
    
    return TabList->GetIsEnabled();
}

bool UMCore_GameMenuHub::IsTabHidden(FGameplayTag TabID) const
{
    if (!TabList || !TabID.IsValid()) return false;
    
    FName TabNameID = FName(*TabID.ToString());
    if (!ScreenWidgets.Contains(TabNameID)) return false;
    
    return TabList->IsInViewport();
}

void UMCore_GameMenuHub::HandleTabSelected(FName TabNameID)
{
    if (!PageSwitcher)
    {
        UE_LOG(LogModulusUI, Error,
       TEXT("HandleTabSelected: Stack is null. Cannot display page content."));
        return;
    }
    
    TObjectPtr<UCommonActivatableWidget>* NamedWidget = ScreenWidgets.Find(TabNameID);
    if (!NamedWidget || !*NamedWidget)
    {
        UE_LOG(LogModulusUI, Warning, TEXT("HandleTabSelected: Tab '%s' not found. Screen may not be registered."),
            *TabNameID.ToString());
        return;
    }

    if (!PageSwitcher->HasChild(*NamedWidget))
    {
        UE_LOG(LogModulusUI, Error,
            TEXT("HandleTabSelected: Widget for tab '%s' is not child of PageSwitcher, rebuilding tab bar"),
            *TabNameID.ToString());

        /** Try to recover by rebuilding */
        RebuildTabBar();
        return;
    }

    /** Deactivate current widgets in target stack */
    PageSwitcher->SetActiveWidget(*NamedWidget);

    UE_LOG(LogModulusUI, Log, TEXT("HandleTabSelected: Successfully switched to tab '%s'"), 
       *TabNameID.ToString());
}
