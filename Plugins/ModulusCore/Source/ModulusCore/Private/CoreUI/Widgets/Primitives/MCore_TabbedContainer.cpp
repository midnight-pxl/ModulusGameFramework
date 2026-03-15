// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUI/Widgets/Primitives/MCore_TabbedContainer.h"

#include "CommonButtonBase.h"
#include "CommonTabListWidgetBase.h"
#include "CommonAnimatedSwitcher.h"
#include "CoreData/Logging/LogModulusUI.h"

UMCore_TabbedContainer::UMCore_TabbedContainer()
{
}

void UMCore_TabbedContainer::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TabList->OnTabSelected.AddDynamic(this, &ThisClass::HandleTabSelected);
}

bool UMCore_TabbedContainer::AddTab(FName TabID, UWidget* PageWidget)
{
	if (TabID.IsNone())
	{
		UE_LOG(LogModulusUI, Warning, TEXT("TabbedContainer::AddTab: Invalid TabID (NAME_None)"));
		return false;
	}

	if (!PageWidget)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("TabbedContainer::AddTab: PageWidget is null for tab '%s'"),
			*TabID.ToString());
		return false;
	}

	if (PageWidgets.Contains(TabID))
	{
		UE_LOG(LogModulusUI, Warning, TEXT("TabbedContainer::AddTab: Tab '%s' already exists, skipping duplicate"),
			*TabID.ToString());
		return false;
	}

	/** Resolve button class */
	TSubclassOf<UCommonButtonBase> ButtonClass = TabButtonClass;
	if (!ButtonClass)
	{
		ButtonClass = UCommonButtonBase::StaticClass();
		UE_LOG(LogModulusUI, Warning,
			TEXT("TabbedContainer::AddTab: TabButtonClass not set, using framework default"));
	}

	PageSwitcher->AddChild(PageWidget);
	TabList->RegisterTab(TabID, ButtonClass, PageWidget, PageWidgets.Num());
	PageWidgets.Add(TabID, PageWidget);

	/** Broadcast OnTabAdded if tab button was created */
	if (UCommonButtonBase* TabButton = TabList->GetTabButtonBaseByID(TabID))
	{
		OnTabAdded.Broadcast(TabID, TabButton);
	}

	UE_LOG(LogModulusUI, Verbose, TEXT("TabbedContainer::AddTab: Added tab '%s' (total: %d)"),
		*TabID.ToString(), PageWidgets.Num());

	return true;
}

bool UMCore_TabbedContainer::RemoveTab(FName TabID)
{
	if (!PageWidgets.Contains(TabID))
	{
		UE_LOG(LogModulusUI, Warning, TEXT("TabbedContainer::RemoveTab: Tab '%s' not found"),
			*TabID.ToString());
		return false;
	}

	/** Store remaining tabs (preserving insertion order via iteration) */
	TArray<TPair<FName, TObjectPtr<UWidget>>> RemainingTabs;
	for (auto& Pair : PageWidgets)
	{
		if (Pair.Key != TabID)
		{
			RemainingTabs.Add(Pair);
		}
	}

	/** Clear everything */
	TabList->RemoveAllTabs();
	PageSwitcher->ClearChildren();
	PageWidgets.Empty();
	SelectedTabID = NAME_None;

	/** Resolve button class */
	TSubclassOf<UCommonButtonBase> ButtonClass = TabButtonClass;
	if (!ButtonClass)
	{
		ButtonClass = UCommonButtonBase::StaticClass();
	}

	/** Re-add remaining tabs */
	for (int32 Index = 0; Index < RemainingTabs.Num(); ++Index)
	{
		const auto& Pair = RemainingTabs[Index];
		PageSwitcher->AddChild(Pair.Value);
		TabList->RegisterTab(Pair.Key, ButtonClass, Pair.Value, Index);
		PageWidgets.Add(Pair.Key, Pair.Value);
	}

	/** Select first remaining tab if any */
	if (RemainingTabs.Num() > 0)
	{
		TabList->SelectTabByID(RemainingTabs[0].Key);
	}

	UE_LOG(LogModulusUI, Log, TEXT("TabbedContainer::RemoveTab: Removed tab '%s' (remaining: %d)"),
		*TabID.ToString(), PageWidgets.Num());

	return true;
}

void UMCore_TabbedContainer::ClearAllTabs()
{
	TabList->RemoveAllTabs();
	PageSwitcher->ClearChildren();
	PageWidgets.Empty();
	SelectedTabID = NAME_None;
}

bool UMCore_TabbedContainer::SelectTab(FName TabID)
{
	if (!PageWidgets.Contains(TabID))
	{
		UE_LOG(LogModulusUI, Warning, TEXT("TabbedContainer::SelectTab: Tab '%s' not found"),
			*TabID.ToString());
		return false;
	}

	TabList->SelectTabByID(TabID);
	SelectedTabID = TabID;

	return true;
}

void UMCore_TabbedContainer::HandleTabSelected(FName TabNameID)
{
	TObjectPtr<UWidget>* FoundWidget = PageWidgets.Find(TabNameID);
	if (!FoundWidget || !*FoundWidget)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("TabbedContainer::HandleTabSelected: Tab '%s' not found in cache"),
			*TabNameID.ToString());
		return;
	}

	if (!PageSwitcher->HasChild(*FoundWidget))
	{
		UE_LOG(LogModulusUI, Error,
			TEXT("TabbedContainer::HandleTabSelected: Widget for tab '%s' is not a child of PageSwitcher"),
			*TabNameID.ToString());
		return;
	}

	PageSwitcher->SetActiveWidget(*FoundWidget);
	SelectedTabID = TabNameID;
	OnTabSelected.Broadcast(TabNameID);

	UE_LOG(LogModulusUI, Verbose, TEXT("TabbedContainer::HandleTabSelected: Switched to tab '%s'"),
		*TabNameID.ToString());
}

UWidget* UMCore_TabbedContainer::GetPageWidget(FName TabID) const
{
	const TObjectPtr<UWidget>* Found = PageWidgets.Find(TabID);
	return Found ? *Found : nullptr;
}

UCommonButtonBase* UMCore_TabbedContainer::GetTabButton(FName TabID) const
{
	return TabList->GetTabButtonBaseByID(TabID);
}

bool UMCore_TabbedContainer::SetTabEnabled(FName TabID, bool bEnabled)
{
	if (!PageWidgets.Contains(TabID))
	{
		UE_LOG(LogModulusUI, Warning, TEXT("TabbedContainer::SetTabEnabled: Tab '%s' not found"),
			*TabID.ToString());
		return false;
	}

	TabList->SetTabEnabled(TabID, bEnabled);
	return true;
}

bool UMCore_TabbedContainer::SetTabHidden(FName TabID, bool bIsHidden)
{
	if (!PageWidgets.Contains(TabID))
	{
		UE_LOG(LogModulusUI, Warning, TEXT("TabbedContainer::SetTabHidden: Tab '%s' not found"),
			*TabID.ToString());
		return false;
	}

	TabList->SetTabVisibility(TabID, bIsHidden ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	return true;
}

bool UMCore_TabbedContainer::IsTabEnabled(FName TabID) const
{
	UCommonButtonBase* TabButton = TabList->GetTabButtonBaseByID(TabID);
	if (!TabButton) return false;

	return TabButton->GetIsEnabled();
}

bool UMCore_TabbedContainer::IsTabHidden(FName TabID) const
{
	UCommonButtonBase* TabButton = TabList->GetTabButtonBaseByID(TabID);
	if (!TabButton) return false;

	return !TabButton->IsVisible();
}
