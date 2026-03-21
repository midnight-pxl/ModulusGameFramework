// Copyright Midnight Pixel Studio LLC. All Rights Reserved.

#include "CoreUI/Widgets/MCore_SettingsPanel.h"

#include "CoreData/DevSettings/MCore_CoreSettings.h"
#include "CoreData/Libraries/MCore_GameSettingsLibrary.h"
#include "CoreData/Types/Settings/MCore_DA_SettingDefinition.h"
#include "CoreData/Types/Settings/MCore_DA_SettingsCollection.h"
#include "CoreUI/MCore_UISubsystem.h"
#include "CoreUI/Widgets/Primitives/MCore_ButtonBase.h"
#include "CoreUI/Widgets/Primitives/MCore_TabbedContainer.h"
#include "CoreUI/Widgets/MCore_SettingsWidget_Base.h"
#include "CoreUI/Widgets/MCore_SettingsWidget_Slider.h"
#include "CoreUI/Widgets/MCore_SettingsWidget_Switcher.h"

#include "CommonTextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "GameplayTagsManager.h"

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

	BuildPanel();

	TabbedContainer_Main->OnTabSelected.AddDynamic(this, &ThisClass::HandleMainTabSelected);

	Btn_ResetAll->OnClicked().AddUObject(this, &ThisClass::HandleResetAllClicked);
	Btn_ResetCategory->OnClicked().AddUObject(this, &ThisClass::HandleResetCategoryClicked);
	Btn_Back->OnClicked().AddUObject(this, &ThisClass::HandleBackClicked);
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
	const UMCore_CoreSettings* CoreSettings = UMCore_CoreSettings::Get();
	if (!CoreSettings || !CoreSettings->DefaultSettingsCollection)
	{
		UE_LOG(LogTemp, Error,
			TEXT("UMCore_SettingsPanel::BuildPanel - CoreSettings or DefaultSettingsCollection is null. "
			     "Ensure UMCore_CoreSettings has a valid DefaultSettingsCollection assigned."));
		return;
	}

	CachedCollection = CoreSettings->DefaultSettingsCollection;

	TabbedContainer_Main->ClearAllTabs();
	TabIDToLeafTag.Reset();
	AllSettingWidgets.Reset();
	KeyBindingMainTabID = NAME_None;
	PreviousMainTabID = NAME_None;

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
			// Routing exception: placeholder page, actual panel pushed on tab selection
			PageWidget = NewObject<USizeBox>(this);
			KeyBindingMainTabID = TabID;
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
		PreviousMainTabID = FirstTabID;
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
		CreateWidget<UMCore_TabbedContainer>(GetOwningPlayer(), SubTabContainerClass);
	if (!SubContainer)
	{
		return nullptr;
	}

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

	// Sort by SortOrder (lower values first)
	Definitions.Sort([](const UMCore_DA_SettingDefinition& A, const UMCore_DA_SettingDefinition& B)
	{
		return A.SortOrder < B.SortOrder;
	});

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
				GetOwningPlayer(), CoreSettings->SettingsSwitcherWidgetClass);
		}
		break;

	case EMCore_SettingType::Slider:
		if (CoreSettings->SettingsSliderWidgetClass)
		{
			Widget = CreateWidget<UMCore_SettingsWidget_Slider>(
				GetOwningPlayer(), CoreSettings->SettingsSliderWidgetClass);
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
	if (bIsRevertingTab)
	{
		return;
	}

	// KeyBinding routing exception: push dedicated panel and revert tab selection
	if (TabID == KeyBindingMainTabID && !KeyBindingMainTabID.IsNone())
	{
		bIsRevertingTab = true;
		TabbedContainer_Main->SelectTab(PreviousMainTabID);
		bIsRevertingTab = false;

		const UMCore_CoreSettings* CoreSettings = UMCore_CoreSettings::Get();
		if (CoreSettings && CoreSettings->KeyBindingPanelClass)
		{
			if (UMCore_UISubsystem* UISubsystem =
				GetOwningLocalPlayer()->GetSubsystem<UMCore_UISubsystem>())
			{
				// TODO: Verify correct layer tag via codebase audit.
				// Hardcoded GameMenu may not match the layer this panel was pushed to.
				UISubsystem->PushWidgetToLayer(
					CoreSettings->KeyBindingPanelClass,
					FGameplayTag::RequestGameplayTag(FName("MCore.UI.Layer.GameMenu")));
			}
		}
		return;
	}

	PreviousMainTabID = TabID;

	// Resolve ActiveLeafCategory from page type
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
	if (!CachedCollection)
	{
		return;
	}

	// TODO: Gate behind UMCore_ConfirmationDialog before executing
	UMCore_GameSettingsLibrary::ResetAllSettingsToDefault(GetOwningLocalPlayer());

	RefreshAllWidgets();
}

void UMCore_SettingsPanel::HandleResetCategoryClicked()
{
	if (!CachedCollection || !ActiveLeafCategory.IsValid())
	{
		return;
	}

	UMCore_GameSettingsLibrary::ResetCategoryToDefault(
		GetOwningLocalPlayer(), ActiveLeafCategory);

	RefreshAllWidgets();
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