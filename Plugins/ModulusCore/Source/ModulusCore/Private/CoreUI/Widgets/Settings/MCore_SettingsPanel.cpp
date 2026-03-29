// Copyright Midnight Pixel Studio LLC. All Rights Reserved.

#include "CoreUI/Widgets/Settings/MCore_SettingsPanel.h"

#include "CoreData/DevSettings/MCore_CoreSettings.h"
#include "CoreData/Libraries/MCore_GameSettingsLibrary.h"
#include "CoreData/Types/Settings/MCore_DA_SettingDefinition.h"
#include "CoreData/Types/Settings/MCore_DA_SettingsCollection.h"
#include "CoreUI/MCore_UISubsystem.h"
#include "CoreUI/Widgets/Primitives/MCore_ButtonBase.h"
#include "CoreUI/Widgets/Primitives/MCore_TabbedContainer.h"
#include "CoreUI/Widgets/Settings/MCore_SettingsWidget_Base.h"
#include "CoreUI/Widgets/Settings/MCore_SettingsWidget_Slider.h"
#include "CoreUI/Widgets/Settings/MCore_SettingsWidget_Switcher.h"
#include "CoreUI/Widgets/KeyBindings/MCore_KeyBindingPanel_Base.h"
#include "CoreUI/Widgets/Primitives/MCore_ConfirmationDialog.h"
#include "CoreData/Logging/LogModulusUI.h"
#include "CoreData/Tags/MCore_UILayerTags.h"
#include "CommonTextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"

// ============================================================================
// FILE-LOCAL HELPERS
// ============================================================================

namespace
{
	FGameplayTag GetParentTag(const FGameplayTag& Tag)
	{
		const FString TagStr = Tag.ToString();
		int32 LastDot;
		if (TagStr.FindLastChar(TEXT('.'), LastDot))
		{
			return FGameplayTag::RequestGameplayTag(FName(*TagStr.Left(LastDot)), false);
		}
		return FGameplayTag();
	}

	FText GetCategoryDisplayName(const UMCore_DA_SettingsCollection* Collection, const FGameplayTag& Tag)
	{
		if (const FText* Name = Collection->CategoryDisplayName.Find(Tag))
		{
			return *Name;
		}
		// Fallback: last segment of the tag path
		const FString TagStr = Tag.ToString();
		int32 LastDot;
		if (TagStr.FindLastChar(TEXT('.'), LastDot))
		{
			return FText::FromString(TagStr.Mid(LastDot + 1));
		}
		return FText::GetEmpty();
	}

	void SetTabButtonLabel(UMCore_TabbedContainer* Container, FName TabID,
		const UMCore_DA_SettingsCollection* Collection, const FGameplayTag& Tag)
	{
		// NOTE: Assumes UMCore_ButtonBase has SetButtonText(). If not, add it or wire in BP.
		if (UMCore_ButtonBase* TabBtn = Cast<UMCore_ButtonBase>(Container->GetTabButton(TabID)))
		{
			TabBtn->SetButtonText(GetCategoryDisplayName(Collection, Tag));
		}
	}
}

// ============================================================================
// LIFECYCLE
// ============================================================================

void UMCore_SettingsPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TabbedContainer_Main->OnTabSelected.AddDynamic(this, &ThisClass::HandleMainTabSelected);

	Btn_ResetAll->OnButtonClicked.AddDynamic(this, &ThisClass::HandleResetAllClicked);
	Btn_ResetCategory->OnButtonClicked.AddDynamic(this, &ThisClass::HandleResetCategoryClicked);
	Btn_Back->OnButtonClicked.AddDynamic(this, &ThisClass::HandleBackClicked);
}

void UMCore_SettingsPanel::NativeOnActivated()
{
	Super::NativeOnActivated();

	UE_LOG(LogModulusUI, Warning, TEXT("SettingsPanel::NativeOnActivated: this=%p, IsActivated()=%s, TabbedContainer_Main=%s, bNeedsFullRebuild=%s"),
		this,
		IsActivated() ? TEXT("true") : TEXT("false"),
		TabbedContainer_Main ? TEXT("valid") : TEXT("NULL"),
		bNeedsFullRebuild ? TEXT("true") : TEXT("false"));

	if (!IsActivated())
	{
		UE_LOG(LogModulusUI, Warning, TEXT("SettingsPanel::NativeOnActivated: EARLY EXIT - not activated (BlockTags?)"));
		return;
	}

	if (bNeedsFullRebuild)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("SettingsPanel::NativeOnActivated: POST-DESTRUCT REBUILD — rebinding child delegates + full BuildPanel"));
		bNeedsFullRebuild = false;

		// Rebind the 4 child delegates that NativeOnInitialized originally bound
		// NativeDestruct unbound all of these — AddDynamic on clean delegates, no double-bind risk
		TabbedContainer_Main->OnTabSelected.AddDynamic(this, &ThisClass::HandleMainTabSelected);
		Btn_ResetAll->OnButtonClicked.AddDynamic(this, &ThisClass::HandleResetAllClicked);
		Btn_ResetCategory->OnButtonClicked.AddDynamic(this, &ThisClass::HandleResetCategoryClicked);
		Btn_Back->OnButtonClicked.AddDynamic(this, &ThisClass::HandleBackClicked);

		// BuildPanel internally clears stale state:
		//   ClearAllTabs() resets PageWidgets/TabList/PageSwitcher (fixes stale GetTabCount=5)
		//   Rebuilds SubContainer->OnTabSelected and Widget->OnSettingFocused delegates via helpers
		BuildPanel();
	}
	else if (TabbedContainer_Main->GetTabCount() == 0)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("SettingsPanel::NativeOnActivated: Calling BuildPanel (first activation)"));
		BuildPanel();
	}
	else
	{
		UE_LOG(LogModulusUI, Warning, TEXT("SettingsPanel::NativeOnActivated: Calling RefreshAllWidgets (re-activation, %d tabs exist)"), TabbedContainer_Main->GetTabCount());
		RefreshAllWidgets();
	}
}

UWidget* UMCore_SettingsPanel::NativeGetDesiredFocusTarget() const
{
	if (TabbedContainer_Main)
	{
		const FName SelectedTab = TabbedContainer_Main->GetSelectedTabID();
		if (!SelectedTab.IsNone())
		{
			if (UWidget* TabButton = TabbedContainer_Main->GetTabButton(SelectedTab))
			{
				return TabButton;
			}
		}
	}

	return Super::NativeGetDesiredFocusTarget();
}

void UMCore_SettingsPanel::NativeOnDeactivated()
{
	UE_LOG(LogModulusUI, Warning, TEXT("SettingsPanel::NativeOnDeactivated: ENTERED - this=%p, TabCount=%d"),
		this,
		TabbedContainer_Main ? TabbedContainer_Main->GetTabCount() : -1);

	// Dismiss any pending confirmation dialog before this panel leaves the stack
	if (PendingConfirmationDialog.IsValid())
	{
		PendingConfirmationDialog->OnDialogResult.RemoveAll(this);
		PendingConfirmationDialog->DeactivateWidget();
		PendingConfirmationDialog.Reset();
	}

	Super::NativeOnDeactivated();
}

void UMCore_SettingsPanel::NativeDestruct()
{
	bNeedsFullRebuild = true;

	UE_LOG(LogModulusUI, Warning, TEXT("SettingsPanel::NativeDestruct: ENTERED - this=%p, TabbedContainer_Main=%s, TabCount=%d"),
		this,
		TabbedContainer_Main ? TEXT("valid") : TEXT("NULL"),
		TabbedContainer_Main ? TabbedContainer_Main->GetTabCount() : -1);

	if (TabbedContainer_Main)
	{
		TabbedContainer_Main->OnTabSelected.RemoveAll(this);
	}

	for (UMCore_TabbedContainer* SubContainer : SubTabContainers)
	{
		if (IsValid(SubContainer))
		{
			SubContainer->OnTabSelected.RemoveAll(this);
		}
	}
	SubTabContainers.Empty();

	for (UMCore_SettingsWidget_Base* Widget : AllSettingWidgets)
	{
		if (IsValid(Widget))
		{
			Widget->OnSettingFocused.RemoveAll(this);
		}
	}
	AllSettingWidgets.Empty();

	if (PendingConfirmationDialog.IsValid())
	{
		PendingConfirmationDialog->OnDialogResult.RemoveAll(this);
		PendingConfirmationDialog.Reset();
	}

	if (Btn_ResetAll) { Btn_ResetAll->OnButtonClicked.RemoveAll(this); }
	if (Btn_ResetCategory) { Btn_ResetCategory->OnButtonClicked.RemoveAll(this); }
	if (Btn_Back) { Btn_Back->OnButtonClicked.RemoveAll(this); }

	Super::NativeDestruct();
}

// ============================================================================
// BLUEPRINT EXTENSION POINTS
// ============================================================================

void UMCore_SettingsPanel::OnCategoryPageCreated_Implementation(
	const FGameplayTag& CategoryTag, UScrollBox* PageScrollBox)
{
	// Default: no-op. Blueprint subclasses can override to inject custom widgets.
}

void UMCore_SettingsPanel::OnPanelBuildComplete_Implementation()
{
	// Default: no-op. Blueprint subclasses can override for post-build setup.
}

// ============================================================================
// PANEL BUILD
// ============================================================================

void UMCore_SettingsPanel::BuildPanel()
{
	UE_LOG(LogModulusUI, Warning, TEXT("BuildPanel: ENTERED - TabbedContainer has %d tabs"), 
	TabbedContainer_Main->GetTabCount());
	
	const UMCore_CoreSettings* CoreSettings = UMCore_CoreSettings::Get();
	if (!CoreSettings || !CoreSettings->GetDefaultSettingsCollection())
	{
		UE_LOG(LogModulusUI, Error,
			TEXT("UMCore_SettingsPanel::BuildPanel - CoreSettings or DefaultSettingsCollection is null. "
			     "Ensure UMCore_CoreSettings has a valid default SettingsCollection assigned."));
		return;
	}

	CachedCollection = CoreSettings->GetDefaultSettingsCollection();

	TabbedContainer_Main->ClearAllTabs();
	TabIDToLeafTag.Reset();
	AllSettingWidgets.Reset();
	SubTabContainers.Reset();

	const TArray<FGameplayTag> AllCategories = CachedCollection->GetAllCategories();

	// Group depth-4 tags under their depth-3 parent, preserving encounter order
	TArray<FGameplayTag> MainTabOrder;
	TMap<FGameplayTag, TArray<FGameplayTag>> ParentToChildren;

	for (const FGameplayTag& LeafTag : AllCategories)
	{
		const FGameplayTag ParentTag = GetParentTag(LeafTag);
		if (!ParentTag.IsValid())
		{
			continue;
		}

		if (!ParentToChildren.Contains(ParentTag))
		{
			MainTabOrder.Add(ParentTag);
		}
		ParentToChildren.FindOrAdd(ParentTag).Add(LeafTag);
	}

	static const FGameplayTag KeyBindingTag =
		FGameplayTag::RequestGameplayTag(FName("MCore.Settings.Category.KeyBinding"), false);

	FName FirstTabID = NAME_None;

	for (const FGameplayTag& ParentTag : MainTabOrder)
	{
		const FName TabID = FName(*ParentTag.ToString());
		UWidget* PageWidget = nullptr;

		if (ParentTag.MatchesTagExact(KeyBindingTag))
		{
			if (CoreSettings->KeyBindingPanelClass)
			{
				PageWidget = CreateWidget<UMCore_KeyBindingPanel_Base>(this, CoreSettings->KeyBindingPanelClass);
			}

			if (!PageWidget)
			{
				UE_LOG(LogModulusUI, Warning, TEXT("KeyBindingPanelClass not set in CoreSettings"));
				PageWidget = NewObject<USizeBox>(this);
			}
		}
		else
		{
			const TArray<FGameplayTag>& Children = ParentToChildren[ParentTag];

			if (Children.Num() == 1)
			{
				PageWidget = BuildSinglePage(CachedCollection, Children[0]);
				TabIDToLeafTag.Add(TabID, Children[0]);
			}
			else
			{
				PageWidget = BuildTabbedPage(CachedCollection, ParentTag, Children);
			}
		}

		UE_LOG(LogModulusUI, Log, TEXT("BuildPanel: Adding tab [%s] order index %d"), *TabID.ToString(), MainTabOrder.IndexOfByKey(ParentTag));
		if (PageWidget && TabbedContainer_Main->AddTab(TabID, PageWidget))
		{
			SetTabButtonLabel(TabbedContainer_Main, TabID, CachedCollection, ParentTag);

			if (FirstTabID.IsNone())
			{
				FirstTabID = TabID;
			}
		}
	}

	if (!FirstTabID.IsNone())
	{
		TabbedContainer_Main->SelectTab(FirstTabID);
	}

	OnPanelBuildComplete();
}

UScrollBox* UMCore_SettingsPanel::BuildSinglePage(
	const UMCore_DA_SettingsCollection* Collection, const FGameplayTag& SubcategoryTag)
{
	UScrollBox* ScrollBox = NewObject<UScrollBox>(this);
	PopulatePage(ScrollBox, Collection, SubcategoryTag);

	OnCategoryPageCreated(SubcategoryTag, ScrollBox);

	return ScrollBox;
}

UMCore_TabbedContainer* UMCore_SettingsPanel::BuildTabbedPage(
	const UMCore_DA_SettingsCollection* Collection,
	const FGameplayTag& ParentTag,
	const TArray<FGameplayTag>& ChildTags)
{
	UMCore_TabbedContainer* SubContainer =
		CreateWidget<UMCore_TabbedContainer>(this, SubTabContainerClass);
	if (!SubContainer)
	{
		return nullptr;
	}

	SubTabContainers.Add(SubContainer);

	FName FirstSubTabID = NAME_None;

	for (const FGameplayTag& ChildTag : ChildTags)
	{
		const FName SubTabID = FName(*ChildTag.ToString());
		UScrollBox* SubPage = NewObject<UScrollBox>(this);
		PopulatePage(SubPage, Collection, ChildTag);

		if (SubContainer->AddTab(SubTabID, SubPage))
		{
			SetTabButtonLabel(SubContainer, SubTabID, Collection, ChildTag);
			TabIDToLeafTag.Add(SubTabID, ChildTag);

			OnCategoryPageCreated(ChildTag, SubPage);

			if (FirstSubTabID.IsNone())
			{
				FirstSubTabID = SubTabID;
			}
		}
	}

	SubContainer->OnTabSelected.AddDynamic(this, &ThisClass::HandleSubTabSelected);

	if (!FirstSubTabID.IsNone())
	{
		SubContainer->SelectTab(FirstSubTabID);
	}

	return SubContainer;
}

void UMCore_SettingsPanel::PopulatePage(
	UScrollBox* ScrollBox, const UMCore_DA_SettingsCollection* Collection,
	const FGameplayTag& CategoryTag)
{
	TArray<UMCore_DA_SettingDefinition*> Definitions = Collection->GetSettingsInCategory(CategoryTag);

	for (const UMCore_DA_SettingDefinition* Definition : Definitions)
	{
		if (!Definition)
		{
			continue;
		}

		if (UMCore_SettingsWidget_Base* Widget = CreateSettingWidget(Definition))
		{
			ScrollBox->AddChild(Widget);
			AllSettingWidgets.Add(Widget);

			Widget->OnSettingFocused.AddDynamic(this, &ThisClass::HandleSettingFocused);
		}
	}
}

// ============================================================================
// WIDGET FACTORY
// ============================================================================

/**
 * @param Definition  Must be non-null. SettingType drives which widget class is spawned.
 * @return            Initialized widget, or nullptr if the widget class is not configured on CoreSettings.
 */
UMCore_SettingsWidget_Base* UMCore_SettingsPanel::CreateSettingWidget(
	const UMCore_DA_SettingDefinition* Definition)
{
	const UMCore_CoreSettings* CoreSettings = UMCore_CoreSettings::Get();
	UMCore_SettingsWidget_Base* Widget = nullptr;

	switch (Definition->SettingType)
	{
	case EMCore_SettingType::Toggle:
	case EMCore_SettingType::Dropdown:
		if (CoreSettings->SettingsSwitcherWidgetClass)
		{
			Widget = CreateWidget<UMCore_SettingsWidget_Switcher>(
				this, CoreSettings->SettingsSwitcherWidgetClass);
		}
		break;

	case EMCore_SettingType::Slider:
		if (CoreSettings->SettingsSliderWidgetClass)
		{
			Widget = CreateWidget<UMCore_SettingsWidget_Slider>(
				this, CoreSettings->SettingsSliderWidgetClass);
		}
		break;
	}

	if (Widget)
	{
		Widget->InitFromDefinition(Definition);
	}

	return Widget;
}

// ============================================================================
// TAB CALLBACKS
// ============================================================================

void UMCore_SettingsPanel::HandleMainTabSelected(FName TabID)
{
	/* Resolve ActiveLeafCategory from page type */
	UWidget* Page = TabbedContainer_Main->GetPageWidget(TabID);

	if (UMCore_TabbedContainer* SubContainer = Cast<UMCore_TabbedContainer>(Page))
	{
		const FName SubTabID = SubContainer->GetSelectedTabID();
		if (const FGameplayTag* LeafTag = TabIDToLeafTag.Find(SubTabID))
		{
			ActiveLeafCategory = *LeafTag;
		}
	}
	else if (const FGameplayTag* LeafTag = TabIDToLeafTag.Find(TabID))
	{
		ActiveLeafCategory = *LeafTag;
	}

	FocusFirstWidgetInActivePage();
}

void UMCore_SettingsPanel::HandleSubTabSelected(FName TabID)
{
	if (const FGameplayTag* LeafTag = TabIDToLeafTag.Find(TabID))
	{
		ActiveLeafCategory = *LeafTag;
	}

	FocusFirstWidgetInActivePage();
}

void UMCore_SettingsPanel::FocusFirstWidgetInActivePage()
{
	const FName ActiveMainTab = TabbedContainer_Main->GetSelectedTabID();
	UWidget* Page = TabbedContainer_Main->GetPageWidget(ActiveMainTab);

	UScrollBox* ScrollBox = nullptr;

	if (UMCore_TabbedContainer* SubContainer = Cast<UMCore_TabbedContainer>(Page))
	{
		const FName ActiveSubTab = SubContainer->GetSelectedTabID();
		ScrollBox = Cast<UScrollBox>(SubContainer->GetPageWidget(ActiveSubTab));
	}
	else
	{
		ScrollBox = Cast<UScrollBox>(Page);
	}

	if (ScrollBox && ScrollBox->GetChildrenCount() > 0)
	{
		if (UWidget* FirstChild = ScrollBox->GetChildAt(0))
		{
			FirstChild->SetUserFocus(GetOwningPlayer());
		}
	}
}

// ============================================================================
// ACTION BAR
// ============================================================================

void UMCore_SettingsPanel::HandleResetAllClicked()
{
	const UMCore_CoreSettings* CoreSettings = UMCore_CoreSettings::Get();
	if (!CoreSettings || !CoreSettings->ConfirmationDialogClass)
	{
		/* No dialog configured, reset directly as fallback */
		UMCore_GameSettingsLibrary::ResetAllSettingsToDefault(GetOwningLocalPlayer());
		RefreshAllWidgets();
		return;
	}
	
	if (UMCore_UISubsystem* UISubsystem = GetOwningLocalPlayer()->GetSubsystem<UMCore_UISubsystem>())
	{
		UCommonActivatableWidget* CAWidget = UISubsystem->OpenScreen(
			CoreSettings->ConfirmationDialogClass,
			MCore_UILayerTags::MCore_UI_Layer_Modal);

		if (UMCore_ConfirmationDialog* Dialog = Cast<UMCore_ConfirmationDialog>(CAWidget))
		{
			PendingConfirmationDialog = Dialog;
			Dialog->SetDialogMessage(
				NSLOCTEXT("ModulusCore", "ResetAllMessage", "Reset all setting to defaults?"));
			Dialog->SetButtonLabels(
				NSLOCTEXT("ModulusCore", "ResetConfirm", "Reset"),
				NSLOCTEXT("ModulusCore", "ResetCancel", "Cancel"));
			Dialog->OnDialogResult.AddDynamic(this, &ThisClass::HandleResetAllConfirmResult);
		}
	}
}

void UMCore_SettingsPanel::HandleResetAllConfirmResult(bool bConfirmed)
{
	PendingConfirmationDialog.Reset();

	if (bConfirmed)
	{
		UMCore_GameSettingsLibrary::ResetAllSettingsToDefault(GetOwningLocalPlayer());
		RefreshAllWidgets();
	}
}

void UMCore_SettingsPanel::HandleResetCategoryClicked()
{
	if (!CachedCollection || !ActiveLeafCategory.IsValid()) { return; }

	const UMCore_CoreSettings* CoreSettings = UMCore_CoreSettings::Get();
	if (!CoreSettings || !CoreSettings->ConfirmationDialogClass)
	{
		UMCore_GameSettingsLibrary::ResetCategoryToDefault(
			GetOwningLocalPlayer(), ActiveLeafCategory);
		RefreshAllWidgets();
		return;
	}
	
	if (UMCore_UISubsystem* UISubsystem = GetOwningLocalPlayer()->GetSubsystem<UMCore_UISubsystem>())
	{
		UCommonActivatableWidget* CAWidget = UISubsystem->OpenScreen(
			CoreSettings->ConfirmationDialogClass,
			MCore_UILayerTags::MCore_UI_Layer_Modal);

		if (UMCore_ConfirmationDialog* Dialog = Cast<UMCore_ConfirmationDialog>(CAWidget))
		{
			PendingConfirmationDialog = Dialog;
			const FText CurrentCategory = GetCategoryDisplayName(
				UMCore_CoreSettings::Get()->GetDefaultSettingsCollection(), ActiveLeafCategory);
			Dialog->SetDialogMessage(
				FText::Format(NSLOCTEXT("ModulusCore", "ResetCategoryMessage",
					"Reset {0} settings to defaults?"), CurrentCategory));
			Dialog->SetButtonLabels(
				NSLOCTEXT("ModulusCore", "ResetConfirm", "Reset"),
				NSLOCTEXT("ModulusCore", "ResetCancel", "Cancel"));
			Dialog->OnDialogResult.AddDynamic(this, &ThisClass::HandleResetCategoryConfirmResult);
		}
	}
}

void UMCore_SettingsPanel::HandleResetCategoryConfirmResult(bool bConfirmed)
{
	PendingConfirmationDialog.Reset();

	if (bConfirmed && ActiveLeafCategory.IsValid())
	{
		UMCore_GameSettingsLibrary::ResetCategoryToDefault(
			GetOwningLocalPlayer(), ActiveLeafCategory);
		RefreshAllWidgets();
	}
}

void UMCore_SettingsPanel::HandleBackClicked()
{
	DeactivateWidget();
}

// ============================================================================
// SETTING FOCUS
// ============================================================================

void UMCore_SettingsPanel::HandleSettingFocused(FGameplayTag SettingTag, FText Description)
{
	if (Txt_SettingDescription)
	{
		Txt_SettingDescription->SetText(Description);
	}
}



// ============================================================================
// REFRESH
// ============================================================================

void UMCore_SettingsPanel::RefreshAllWidgets()
{
	for (UMCore_SettingsWidget_Base* Widget : AllSettingWidgets)
	{
		if (Widget)
		{
			Widget->RefreshValueFromSettings();
		}
	}
}