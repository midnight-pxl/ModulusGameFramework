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

void UMCore_UISubsystem::RegisterMenuScreen(TSubclassOf<UCommonActivatableWidget> ScreenWidgetClass, const FText& TabName,
	int32 Priority, UTexture2D* TabIcon)
{
	if (!ScreenWidgetClass)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("RegisterMenuScreen: ScreenWidgetClass is null"));
		return;
	}

	if (TabName.IsEmpty())
	{
		UE_LOG(LogModulusUI, Warning, TEXT("RegisterMenuScreen: TabName is empty for %s"),
			*ScreenWidgetClass->GetName());
		return;
	}

	if (IsScreenRegistered(ScreenWidgetClass))
	{
		UE_LOG(LogModulusUI, Log, TEXT("Screen %s already registered in menu hub"),
			*ScreenWidgetClass->GetName());
		return;
	}

	// Create new tab info
	FMCore_MenuTab NewTab;
	NewTab.TabName = TabName;
	NewTab.TabIcon = TabIcon;
	NewTab.ScreenWidgetClass = ScreenWidgetClass;  // ← Consistent naming
	NewTab.Priority = Priority;

	// Add and sort
	RegisteredMenuScreens.Add(NewTab);
	RegisteredMenuScreens.Sort();

	UE_LOG(LogModulusUI, Verbose, TEXT("Registered menu screen '%s' (Priority: %d, TabID: %s)"),
	*TabName.ToString(), Priority, *NewTab.TabID.ToString());
}

bool UMCore_UISubsystem::IsScreenRegistered(TSubclassOf<UCommonActivatableWidget> ScreenWidgetClass) const
{
	if (!ScreenWidgetClass) { return false; }

	return RegisteredMenuScreens.ContainsByPredicate([ScreenWidgetClass](const FMCore_MenuTab& InMenuTab)
	{
		return InMenuTab.ScreenWidgetClass == ScreenWidgetClass;
	});
}
