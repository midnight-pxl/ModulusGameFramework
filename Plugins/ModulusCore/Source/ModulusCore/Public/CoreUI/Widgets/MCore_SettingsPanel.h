// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Primitives/MCore_ActivatableBase.h"
#include "GameplayTagContainer.h"
#include "MCore_SettingsPanel.generated.h"

class UMCore_TabbedContainer;
class UMCore_ButtonBase;
class UMCore_ConfirmationDialog;
class UMCore_DA_SettingDefinition;
class UMCore_DA_SettingsCollection;
class UMCore_SettingsWidget_Base;
class UCommonTextBlock;
class UScrollBox;

/**
 * Data-driven settings panel for auto-generating category tabs
 * and setting widgets from DefaultSettingsCollection in
 * UMCore_CoreSettings
 * 
 * Depth-3 tags become main tabs (Settings_Category_Video, etc.)
 * If Depth-4 tags >1 sub-tabbing occurs (Settings_Category_Video_Display, Settings_Category_Video_GraphicQuality, etc.)
 * KeyBinding category creates an inline KeyBindingPanel_Base widget instead of setting rows.
 */
UCLASS()
class MODULUSCORE_API UMCore_SettingsPanel : public UMCore_ActivatableBase
{
	GENERATED_BODY()
	
public:
 
	/** Re-reads current values from the settings library and updates all setting widgets. */
	UFUNCTION(BlueprintCallable, Category = "Modulus|Settings")
	void RefreshAllWidgets();
 
protected:
 
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
 
	// ============================================================================
	// CONFIGURATION
	// ============================================================================
 
	/** Blueprint class spawned at runtime for nested sub-tab containers. */
	UPROPERTY(EditDefaultsOnly, Category = "Settings Panel")
	TSubclassOf<UMCore_TabbedContainer> SubTabContainerClass;
 
	// ============================================================================
	// BIND WIDGETS
	// ============================================================================
 
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMCore_TabbedContainer> TabbedContainer_Main;
 
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Txt_SettingDescription;
 
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMCore_ButtonBase> Btn_ResetAll;
 
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMCore_ButtonBase> Btn_ResetCategory;
 
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMCore_ButtonBase> Btn_Back;
	
	// ============================================================================
	// BLUEPRINT EXTENSION POINTS
	// ============================================================================
	
	/** Called after each category page is built and populated with setting widgets */
	UFUNCTION(BlueprintNativeEvent, Category = "Modulus|Settings")
	void OnCategoryPageCreated(const FGameplayTag& CategoryTag,
		UScrollBox* PageScrollBox);
	
	/** Called after the full panel build completes. All tabs + pages ready */
	UFUNCTION(BlueprintNativeEvent, Category = "Modulus|Settings")
	void OnPanelBuildComplete();
 
private:
 
	// ============================================================================
	// PANEL BUILD
	// ============================================================================
 
	void BuildPanel();
 
	UScrollBox* BuildSinglePage(const UMCore_DA_SettingsCollection* Collection, const FGameplayTag& SubcategoryTag);
 
	UMCore_TabbedContainer* BuildTabbedPage(const UMCore_DA_SettingsCollection* Collection,
		const FGameplayTag& ParentTag, const TArray<FGameplayTag>& ChildTags);
 
	void PopulatePage(UScrollBox* ScrollBox, const UMCore_DA_SettingsCollection* Collection,
		const FGameplayTag& CategoryTag);
 
	UMCore_SettingsWidget_Base* CreateSettingWidget(const UMCore_DA_SettingDefinition* Definition);
 
	// ============================================================================
	// TAB CALLBACKS
	// ============================================================================
 
	UFUNCTION()
	void HandleMainTabSelected(FName TabID);
 
	UFUNCTION()
	void HandleSubTabSelected(FName TabID);
 
	void FocusFirstWidgetInActivePage();
 
	// ============================================================================
	// ACTION BAR
	// ============================================================================
 
	UFUNCTION()
	void HandleResetAllClicked();
	
	UFUNCTION()
	void HandleResetAllConfirmResult(bool bConfirmed);
 
	UFUNCTION()
	void HandleResetCategoryClicked();
	
	UFUNCTION()
	void HandleResetCategoryConfirmResult(bool bConfirmed);
 
	UFUNCTION()
	void HandleBackClicked();
 
	// ============================================================================
	// SETTING FOCUS
	// ============================================================================

	UFUNCTION()
	void HandleSettingFocused(FGameplayTag SettingTag, FText Description);
 
	// ============================================================================
	// INTERNAL STATE
	// ============================================================================
 
	/* Set once in BuildPanel, used by action bar handlers */
	const UMCore_DA_SettingsCollection* CachedCollection{nullptr};
	
	/**
	 * Maps single-page tab IDs and sub-tab IDs to their depth-4 leaf category tag.
	 * Sub-tabbed main tab IDs are NOT in this map,
	 * query their nested container for Parent instead.
	 */
	TMap<FName, FGameplayTag> TabIDToLeafTag;
 
	FGameplayTag ActiveLeafCategory;
 
	UPROPERTY()
	TArray<TObjectPtr<UMCore_SettingsWidget_Base>> AllSettingWidgets;

	UPROPERTY()
	TArray<TObjectPtr<UMCore_TabbedContainer>> SubTabContainers;

	TWeakObjectPtr<UMCore_ConfirmationDialog> PendingConfirmationDialog;

	bool bNeedsFullRebuild{false};
};
