// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_KeyBindingPanel_Base.h
 *
 * Key binding panel that queries remappable actions and builds grouped rows.
 * Displayed inline in the Settings Panel's KeyBinding category tab.
 */

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MCore_KeyBindingPanel_Base.generated.h"

class UScrollBox;
class UMCore_KeyBindingRow;
class UInputAction;
class APlayerController;

/**
 * Key binding panel that populates a ScrollBox with category headers and binding rows.
 * Self-populates on initialization using the active Enhanced Input key profile.
 *
 * Requires BindWidget: ScrollBox_Bindings.
 */
UCLASS(Abstract, Blueprintable)
class MODULUSCORE_API UMCore_KeyBindingPanel_Base : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	// ====================================================================
	// PUBLIC API
	// ====================================================================

	/** Query all remappable actions, group by category, and spawn rows into the ScrollBox. */
	UFUNCTION(BlueprintCallable, Category = "UI|KeyBinding")
	void PopulateBindings(APlayerController* PC);

	/** Refresh all spawned rows to reflect current bindings. */
	UFUNCTION(BlueprintCallable, Category = "UI|KeyBinding")
	void RefreshAllRows();

	// ====================================================================
	// BLUEPRINT HOOKS
	// ====================================================================

	/** Fires after each category header is added to the ScrollBox. */
	UFUNCTION(BlueprintNativeEvent, Category = "UI|KeyBinding")
	void OnCategoryHeaderCreated(const FText& CategoryDisplayName, UWidget* HeaderWidget);
	virtual void OnCategoryHeaderCreated_Implementation(const FText& CategoryDisplayName, UWidget* HeaderWidget) {}

	/** Fires after each row is created and added to the ScrollBox. */
	UFUNCTION(BlueprintNativeEvent, Category = "UI|KeyBinding")
	void OnRowCreated(UMCore_KeyBindingRow* Row);
	virtual void OnRowCreated_Implementation(UMCore_KeyBindingRow* Row) {}

protected:

	// ====================================================================
	// BIND WIDGETS
	// ====================================================================

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_Bindings;

	// ====================================================================
	// CONFIGURATION
	// ====================================================================

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|KeyBinding")
	TSubclassOf<UMCore_KeyBindingRow> KeyBindingRowClass;

	// ====================================================================
	// LIFECYCLE
	// ====================================================================

	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:

	// ====================================================================
	// HELPERS
	// ====================================================================

	UWidget* CreateCategoryHeader(const FText& CategoryDisplayName);

	UFUNCTION()
	void HandleRowRebindCompleted();

	// ====================================================================
	// STATE
	// ====================================================================

	UPROPERTY(Transient)
	TArray<TObjectPtr<UMCore_KeyBindingRow>> AllRows;
};
