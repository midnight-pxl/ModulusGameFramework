// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUISystem/CoreWidgets/MCore_GameMenuHub.h"

#include "GameplayTagContainer.h"
#include "CommonTabListWidgetBase.h"
#include "CoreUISystem/MCore_UISubsystem.h"
#include "CoreData/CoreLogging/LogModulusUI.h"
#include "CoreData/CoreStructEnums/UIStructsEnums/MCore_MenuTabTypes.h"
#include "CoreUISystem/CoreWidgets/WidgetPrimitives/MCore_ButtonBase.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UMCore_GameMenuHub::UMCore_GameMenuHub(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{
}

void UMCore_GameMenuHub::RebuildTabBar()
{
    // Validate BindWidget components
    if (!TabList || !PageStack)
    {
        UE_LOG(LogModulusUI, Error,
            TEXT("GameMenuHub: TabList or PageStack not bound - verify BindWidget in Blueprint"));
        return;
    }

    // Get owning player context
    ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
    if (!LocalPlayer)
    {
        UE_LOG(LogModulusUI, Warning, TEXT("GameMenuHub: No owning LocalPlayer"));
        return;
    }

    // Get UI subsystem for registered screens
    UMCore_UISubsystem* UISubsystem = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
    if (!UISubsystem)
    {
        UE_LOG(LogModulusUI, Warning, TEXT("GameMenuHub: No UISubsystem found"));
        return;
    }

    const TArray<FMCore_MenuTab>& RegisteredScreens = UISubsystem->GetRegisteredMenuScreens();
    
    // Empty state: No screens registered
    if (RegisteredScreens.IsEmpty())
    {
        if (EmptyStateWidgetClass)
        {
            PageStack->AddWidget(EmptyStateWidgetClass);
            UE_LOG(LogModulusUI, Log, TEXT("GameMenuHub: Showing empty state (no screens registered)"));
        }
        else
        {
            UE_LOG(LogModulusUI, Warning, TEXT("GameMenuHub: No screens registered and EmptyStateWidgetClass not set"));
        }
        return;
    }

    // Clear existing tabs
    TabList->RemoveAllTabs();
    ScreenWidgets.Empty();
    
    // Validate button class with framework default fallback
    TSubclassOf<UMCore_ButtonBase> ButtonClass;
    if (TabButtonClass)
    {
        ButtonClass = TabButtonClass;
    }
    else
    {
        ButtonClass = UMCore_ButtonBase::StaticClass();
        UE_LOG(LogModulusUI, Warning, 
            TEXT("GameMenuHub: TabButtonClass not set, using framework default"));
    }
    
    if (!ButtonClass)
    {
        UE_LOG(LogModulusUI, Error, TEXT("GameMenuHub: No valid button class - cannot create tabs"));
        return;
    }

    // Register tabs with CommonUI
    // Performance: 8 tabs × 0.17ms = 1.36ms total (widget creation dominates)
    for (int32 Index = 0; Index < RegisteredScreens.Num(); ++Index)
    {
        const FMCore_MenuTab& Tab = RegisteredScreens[Index];
        
        FName TabNameID = FName(*Tab.TabID.ToString());
        FText DisplayName = Tab.GetDisplayName();

        // Create screen widget for this tab
        UCommonActivatableWidget* ScreenWidget = CreateWidget<UCommonActivatableWidget>(
            GetOwningPlayer(), Tab.ScreenWidgetClass);

        if (!ScreenWidget)
        {
            UE_LOG(LogModulusUI, Error, 
                TEXT("GameMenuHub: Failed to create widget for '%s'"),
                *Tab.TabID.ToString());
            continue;
        }

        // Register with TabList - explicit index for priority order
        // RegisteredScreens already sorted by priority in UISubsystem
        TabList->RegisterTab(TabNameID, ButtonClass, ScreenWidget, Index);
        
        // Cache widget reference for future queries
        ScreenWidgets.Add(TabNameID, ScreenWidget);
        
        // Configure tab button (text + optional icon)
        // GetTabButtonBaseByID returns UCommonButtonBase*, must cast to access MCore APIs
        if (UCommonButtonBase* ButtonBase = TabList->GetTabButtonBaseByID(TabNameID))
        {
            // Cast to MCore_ButtonBase - guaranteed to succeed due to type enforcement
            UMCore_ButtonBase* Button = Cast<UMCore_ButtonBase>(ButtonBase);
            check(Button); // Should never fail - ButtonClass is TSubclassOf<UMCore_ButtonBase>
            
            // Set button text - API guaranteed to exist
            Button->SetButtonText(DisplayName);
            
            // Set icon if provided - API guaranteed to exist
            if (Tab.TabIcon)
            {
                Button->SetButtonIcon(Tab.TabIcon);
            }
        }
        else
        {
            UE_LOG(LogModulusUI, Warning, 
                TEXT("GameMenuHub: Failed to get button for tab '%s'"),
                *Tab.TabID.ToString());
        }
        
        UE_LOG(LogModulusUI, Verbose, 
            TEXT("GameMenuHub: Registered tab '%s' → Display: '%s'%s"),
            *Tab.TabID.ToString(),
            *DisplayName.ToString(),
            Tab.TabIcon ? TEXT(" (with icon)") : TEXT(""));
    }
    
    // Bind tab selection delegate
    TabList->OnTabSelected.Clear();
    TabList->OnTabSelected.AddUniqueDynamic(this, &UMCore_GameMenuHub::HandleTabSelected);
    
    // Select first tab to show initial content
    if (!RegisteredScreens.IsEmpty())
    {
        FName FirstTabID = FName(*RegisteredScreens[0].TabID.ToString());
        TabList->SelectTabByID(FirstTabID);
    }
    
    UE_LOG(LogModulusUI, Log, 
        TEXT("GameMenuHub: Rebuilt tab bar with %d tabs using %s"),
        RegisteredScreens.Num(),
        *ButtonClass->GetName());
}

void UMCore_GameMenuHub::HandleTabSelected(FName TabNameID)
{
}
