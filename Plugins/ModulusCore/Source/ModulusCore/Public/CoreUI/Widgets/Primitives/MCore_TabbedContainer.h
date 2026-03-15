// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_TabbedContainer.h
 *
 * Domain-agnostic tabbed container primitive providing a tab bar + page switcher pattern.
 * Wraps CommonUI's UCommonTabListWidgetBase and UCommonAnimatedSwitcher as a coordinated unit.
 */

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
 * Key Features:
 * - Manages CommonUI tab list and animated switcher as a coordinated unit
 * - Caller owns page widget creation and lifecycle
 * - Tab enable/disable and show/hide state control
 *
 * Required widget bindings (BindWidget):
 * - TabList: UCommonTabListWidgetBase for button creation and navigation
 * - PageSwitcher: UCommonAnimatedSwitcher for page display
 *
 * Blueprint Usage:
 * 1. Create Blueprint extending this class with the two required widgets
 * 2. Set TabButtonClass to your tab button Blueprint class
 * 3. Call AddTab() to populate tabs with pre-created page widgets
 */
UCLASS(Abstract, Blueprintable, BlueprintType, meta=(DisableNativeTick))
class MODULUSCORE_API UMCore_TabbedContainer : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UMCore_TabbedContainer();

	// ============================================================================
	// CORE API
	// ============================================================================

	/**
	 * Add a tab with a pre-created page widget.
	 * Tabs display in insertion order. Caller owns widget creation and lifecycle.
	 *
	 * @param TabID - Unique FName identifier for this tab
	 * @param PageWidget - Pre-created widget to display when tab is selected
	 * @return true if tab was added successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Tabbed Container")
	bool AddTab(FName TabID, UWidget* PageWidget);

	/**
	 * Remove a tab and its page from the container.
	 * Does NOT destroy the page widget (caller owns lifecycle).
	 *
	 * @param TabID - Tab to remove
	 * @return true if tab was found and removed
	 */
	UFUNCTION(BlueprintCallable, Category = "Tabbed Container")
	bool RemoveTab(FName TabID);

	/**
	 * Remove all tabs and pages. Does NOT destroy page widgets.
	 */
	UFUNCTION(BlueprintCallable, Category = "Tabbed Container")
	void ClearAllTabs();

	/**
	 * Select a tab by ID, switching the page switcher to its page.
	 *
	 * @param TabID - Tab to select
	 * @return true if tab was found and selected
	 */
	UFUNCTION(BlueprintCallable, Category = "Tabbed Container")
	bool SelectTab(FName TabID);

	// ============================================================================
	// QUERIES
	// ============================================================================

	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	FName GetSelectedTabID() const { return SelectedTabID; }

	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	int32 GetTabCount() const { return PageWidgets.Num(); }

	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	bool HasTab(FName TabID) const { return PageWidgets.Contains(TabID); }

	/**
	 * Get the page widget for a given tab.
	 *
	 * @param TabID - Tab to look up
	 * @return Page widget, or nullptr if not found
	 */
	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	UWidget* GetPageWidget(FName TabID) const;

	/**
	 * Get the tab button for a given tab.
	 *
	 * @param TabID - Tab to look up
	 * @return Tab button, or nullptr if not found
	 */
	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	UCommonButtonBase* GetTabButton(FName TabID) const;

	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	UCommonAnimatedSwitcher* GetPageSwitcher() const { return PageSwitcher; }

	// ============================================================================
	// TAB STATE
	// ============================================================================

	UFUNCTION(BlueprintCallable, Category = "Tabbed Container")
	bool SetTabEnabled(FName TabID, bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Tabbed Container")
	bool SetTabHidden(FName TabID, bool bIsHidden);

	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	bool IsTabEnabled(FName TabID) const;

	UFUNCTION(BlueprintPure, Category = "Tabbed Container")
	bool IsTabHidden(FName TabID) const;

	// ============================================================================
	// EVENTS
	// ============================================================================

	UPROPERTY(BlueprintAssignable, Category = "Tabbed Container|Events")
	FOnTabbedContainerTabAdded OnTabAdded;

	UPROPERTY(BlueprintAssignable, Category = "Tabbed Container|Events")
	FOnTabbedContainerTabSelected OnTabSelected;

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTabListWidgetBase> TabList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonAnimatedSwitcher> PageSwitcher;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tabbed Container")
	TSubclassOf<UCommonButtonBase> TabButtonClass;

private:
	UFUNCTION()
	void HandleTabSelected(FName TabNameID);

	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<UWidget>> PageWidgets;

	FName SelectedTabID;
};
