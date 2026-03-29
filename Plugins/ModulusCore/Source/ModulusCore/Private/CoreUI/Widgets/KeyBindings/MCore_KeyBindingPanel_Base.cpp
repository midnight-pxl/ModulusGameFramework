// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUI/Widgets/KeyBindings/MCore_KeyBindingPanel_Base.h"
#include "CoreUI/Widgets/KeyBindings/MCore_KeyBindingRow.h"
#include "CoreData/Libraries/MCore_EnhancedInputDisplay.h"
#include "CoreData/DevSettings/MCore_CoreSettings.h"
#include "CoreData/Logging/LogModulusUI.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"

// ============================================================================
// LIFECYCLE
// ============================================================================

void UMCore_KeyBindingPanel_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PopulateBindings(GetOwningPlayer());
}

void UMCore_KeyBindingPanel_Base::NativeDestruct()
{
	for (UMCore_KeyBindingRow* Row : AllRows)
	{
		if (Row)
		{
			Row->OnRowRebindCompleted.RemoveAll(this);
		}
	}

	Super::NativeDestruct();
}

// ============================================================================
// PUBLIC API
// ============================================================================

void UMCore_KeyBindingPanel_Base::PopulateBindings(APlayerController* PC)
{
	if (!PC || !ScrollBox_Bindings || !KeyBindingRowClass)
	{
		UE_LOG(LogModulusUI, Warning,
			TEXT("[%s] PopulateBindings: Missing PC, ScrollBox, or KeyBindingRowClass"),
			*GetName());
		return;
	}

	// Clear previous state
	ScrollBox_Bindings->ClearChildren();
	for (UMCore_KeyBindingRow* Row : AllRows)
	{
		if (Row) { Row->OnRowRebindCompleted.RemoveAll(this); }
	}
	AllRows.Reset();

	// Query all remappable actions
	TArray<FPlayerKeyMapping> AllMappings =
		UMCore_EnhancedInputDisplay::GetAllRemappableActions(PC);

	if (AllMappings.IsEmpty())
	{
		UE_LOG(LogModulusUI, Log, TEXT("[%s] No remappable actions found"), *GetName());
		return;
	}

	// Deduplicate by mapping name (multiple entries per action for different slots/devices)
	TMap<FName, const FPlayerKeyMapping*> UniqueActions;
	for (const FPlayerKeyMapping& Mapping : AllMappings)
	{
		const FName Name = Mapping.GetMappingName();
		if (!UniqueActions.Contains(Name))
		{
			UniqueActions.Add(Name, &Mapping);
		}
	}

	// Group by display category
	TMap<FString, TArray<const FPlayerKeyMapping*>> CategorizedActions;
	for (const auto& Pair : UniqueActions)
	{
		const FText Category = Pair.Value->GetDisplayCategory();
		const FString CategoryKey = Category.ToString();
		CategorizedActions.FindOrAdd(CategoryKey).Add(Pair.Value);
	}

	// Sort categories alphabetically
	TArray<FString> SortedCategories;
	CategorizedActions.GetKeys(SortedCategories);
	SortedCategories.Sort();

	const bool bShowSecondary = UMCore_CoreSettings::Get()
		? UMCore_CoreSettings::Get()->bShowSecondaryBindings
		: false;

	// Build the ScrollBox content
	for (const FString& CategoryKey : SortedCategories)
	{
		const FText CategoryDisplayName = FText::FromString(CategoryKey);

		UWidget* Header = CreateCategoryHeader(CategoryDisplayName);
		if (Header)
		{
			ScrollBox_Bindings->AddChild(Header);
			OnCategoryHeaderCreated(CategoryDisplayName, Header);
		}

		for (const FPlayerKeyMapping* Mapping : CategorizedActions[CategoryKey])
		{
			UMCore_KeyBindingRow* Row = CreateWidget<UMCore_KeyBindingRow>(
				this, KeyBindingRowClass);
			if (!Row) { continue; }

			const UInputAction* Action = Mapping->GetAssociatedInputAction();
			if (!Action)
			{
				UE_LOG(LogModulusUI, Warning,
					TEXT("[%s] Mapping '%s' has no associated InputAction"),
					*GetName(), *Mapping->GetMappingName().ToString());
				continue;
			}

			Row->InitFromAction(PC, const_cast<UInputAction*>(Action), bShowSecondary);
			Row->OnRowRebindCompleted.AddDynamic(this, &ThisClass::HandleRowRebindCompleted);

			ScrollBox_Bindings->AddChild(Row);
			AllRows.Add(Row);

			OnRowCreated(Row);
		}
	}

	UE_LOG(LogModulusUI, Log, TEXT("[%s] Populated %d binding rows across %d categories"),
		*GetName(), AllRows.Num(), SortedCategories.Num());
}

void UMCore_KeyBindingPanel_Base::RefreshAllRows()
{
	for (UMCore_KeyBindingRow* Row : AllRows)
	{
		if (Row) { Row->RefreshDisplay(); }
	}
}

// ============================================================================
// HELPERS
// ============================================================================

UWidget* UMCore_KeyBindingPanel_Base::CreateCategoryHeader(const FText& CategoryDisplayName)
{
	UTextBlock* Header = NewObject<UTextBlock>(this);
	if (Header)
	{
		Header->SetText(CategoryDisplayName);
	}
	return Header;
}

void UMCore_KeyBindingPanel_Base::HandleRowRebindCompleted()
{
	RefreshAllRows();
}
