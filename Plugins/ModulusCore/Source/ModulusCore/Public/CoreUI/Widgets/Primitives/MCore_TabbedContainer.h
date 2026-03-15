// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MCore_TabbedContainer.generated.h"

class UCommonButtonBase;
class UCommonAnimatedSwitcher;
class UCommonTabListWidgetBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTabbedContainerTabAdded, FName, TabID, UCommonButtonBase*, TabButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTabbedContainerTabSelected, FName, TabID);

/**
 * Reusable tabbed container primitive providing a tab bar + page switcher pattern.
 *
 * Domain-agnostic compound widget. Manages CommonUI's UCommonTabListWidgetBase
 * and UCommonAnimatedSwitcher as a coordinated unit. Caller creates page widgets
 * and passes them in via AddTab — this widget handles structural tab registration,
 * page switching, and tab state.
 *
 * Usage:
 * 1. Create Blueprint extending this class
 * 2. Add UCommonTabListWidgetBase named "TabList" (required)
 * 3. Add UCommonAnimatedSwitcher named "PageSwitcher" (required)
 * 4. Set TabButtonClass to your tab button Blueprint class
 * 5. Call AddTab() to populate tabs with pre-created page widgets
 */
UCLASS(Abstract, Blueprintable, BlueprintType, meta=(DisableNativeTick))
class MODULUSCORE_API UMCore_TabbedContainer : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UMCore_TabbedContainer();

	// --- Core API ---

	/**
	 * Add a tab with a pre-created page widget.
	 * Tabs display in insertion order. Caller owns widget creation and lifecycle.
	 *
	 * @param TabID Unique FName identifier for this tab
	 * @param PageWidget Pre-created widget to display when tab is selected
	 * @return true if tab was added successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Tabbed Container")
	bool AddTab(FName TabID, UWidget* PageWidget);

	/**
	 * Remove a tab and its page from the container.
	 * Does NOT destroy the page widget (caller owns lifecycle).
	 *
	 * @param TabID Tab to remove
	 * @return true if tab was found and removed
	 */
	UFUNCTION(BlueprintCallable, Category = "Tabbed Container")
	bool RemoveTab(FName TabID);

	/** Remove all tabs and pages. Does NOT destroy page widgets. */
	UFUNCTION(BlueprintCallable, Category = "Tabbed Container")
	void ClearAllTabs();

	/**
	 * Select a tab by ID, switching the page switcher to its page.
	 *
	 * @param TabID Tab to select
	 * @return true if tab was found and selected
	 */
	UFUNCTION(BlueprintCallable, Category = "Tabbed Container")
	bool SelectTab(FName TabID);

	// --- Queries ---

	/** Get the currently selected tab ID. Returns NAME_None if nothing selected. */
	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	FName GetSelectedTabID() const { return SelectedTabID; }

	/** Returns the number of registered tabs. */
	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	int32 GetTabCount() const { return PageWidgets.Num(); }

	/** Returns true if a tab with the given ID exists. */
	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	bool HasTab(FName TabID) const { return PageWidgets.Contains(TabID); }

	/** Get the page widget for a given tab. Returns nullptr if not found. */
	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	UWidget* GetPageWidget(FName TabID) const;

	/** Get the tab button for a given tab. Returns nullptr if not found. */
	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	UCommonButtonBase* GetTabButton(FName TabID) const;

	/** Public accessor for the page switcher (e.g., for empty-state content by composing widgets). */
	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	UCommonAnimatedSwitcher* GetPageSwitcher() const { return PageSwitcher; }

	// --- Tab State ---

	UFUNCTION(BlueprintCallable, Category = "Tabbed Container")
	bool SetTabEnabled(FName TabID, bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Tabbed Container")
	bool SetTabHidden(FName TabID, bool bIsHidden);

	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	bool IsTabEnabled(FName TabID) const;

	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	bool IsTabHidden(FName TabID) const;

	// --- Events ---

	/** Fired after AddTab succeeds and the tab button is created */
	UPROPERTY(BlueprintAssignable, Category = "Tabbed Container|Events")
	FOnTabbedContainerTabAdded OnTabAdded;

	/** Fired when a tab is selected (by user interaction or SelectTab call) */
	UPROPERTY(BlueprintAssignable, Category = "Tabbed Container|Events")
	FOnTabbedContainerTabSelected OnTabSelected;

protected:
	//~ UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~ End UUserWidget Interface

	/** CommonUI tab list — handles button creation, clicks, visual states, gamepad nav */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTabListWidgetBase> TabList;

	/** Animated widget switcher for displaying the active tab's page */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonAnimatedSwitcher> PageSwitcher;

	/** Button class used for all tabs. Set in Blueprint defaults. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tabbed Container")
	TSubclassOf<UCommonButtonBase> TabButtonClass;

private:
	/** Bound to TabList->OnTabSelected in NativeOnInitialized */
	UFUNCTION()
	void HandleTabSelected(FName TabNameID);

	/** Cached page widgets keyed by tab ID */
	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<UWidget>> PageWidgets;

	/** Currently selected tab */
	FName SelectedTabID;
};
