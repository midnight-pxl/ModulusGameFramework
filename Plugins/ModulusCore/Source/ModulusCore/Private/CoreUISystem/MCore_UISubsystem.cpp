// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUISystem/MCore_UISubsystem.h"
#include "CoreUISystem/MCore_PrimaryGameLayout.h"
#include "CoreData/CoreLogging/LogModulusUI.h"

UMCore_PrimaryGameLayout* UMCore_UISubsystem::GetPrimaryGameLayout() const
{
	if (CachedPrimaryGameLayout.IsValid())
	{
		return CachedPrimaryGameLayout.Get();
	}
    
	UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem: No PrimaryGameLayout registered for this local player"));
	return nullptr;
}

bool UMCore_UISubsystem::RegisterPrimaryGameLayout(UMCore_PrimaryGameLayout* InLayout)
{
	if (!InLayout)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem: Attempted to register null PrimaryGameLayout"));
		return false;
	}

	CachedPrimaryGameLayout = InLayout;
	UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: Registered PrimaryGameLayout"));
	return true;
}

FGuid UMCore_UISubsystem::RegisterMenuTab(const FMCore_MenuTab& Tab)
{
	if (!Tab.PageWidget)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem: Cannot register menu tab '%s' - PageWidget is null"),
			*Tab.TabLabel.ToString());
		return FGuid();
	}

	// Check for duplicate registration by PageWidget class
	for (const FMCore_MenuTab& ExistingTab : RegisteredMenuTabs)
	{
		if (ExistingTab.PageWidget == Tab.PageWidget)
		{
			UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem: Menu tab already registered for widget class '%s'"),
				*Tab.PageWidget->GetName());
			return ExistingTab.TabID;
		}
	}

	// Create registered copy with guaranteed unique ID
	FMCore_MenuTab RegisteredTab = Tab;
	if (!RegisteredTab.TabID.IsValid())
	{
		RegisteredTab.TabID = FGuid::NewGuid();
	}

	// Add and sort by priority
	RegisteredMenuTabs.Add(RegisteredTab);
	RegisteredMenuTabs.Sort();

	UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: Registered menu tab '%s' (Priority: %d, ID: %s)"),
		*RegisteredTab.TabLabel.ToString(),
		RegisteredTab.Priority,
		*RegisteredTab.TabID.ToString());

	return RegisteredTab.TabID;
}

bool UMCore_UISubsystem::UnregisterMenuTab(TSubclassOf<UCommonActivatableWidget> PageWidget)
{
	if (!PageWidget)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem: Cannot unregister menu tab - PageWidget is null"));
		return false;
	}

	const int32 RemovedCount = RegisteredMenuTabs.RemoveAll([PageWidget](const FMCore_MenuTab& Tab)
	{
		return Tab.PageWidget == PageWidget;
	});

	if (RemovedCount > 0)
	{
		UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: Unregistered %d menu tab(s) for widget class '%s'"),
			RemovedCount, *PageWidget->GetName());
		return true;
	}

	UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem: No menu tab found for widget class '%s'"),
		*PageWidget->GetName());
	
	return false;
}

TArray<FMCore_MenuTab> UMCore_UISubsystem::GetRegisteredMenuTabs() const
{
	return RegisteredMenuTabs;
}
