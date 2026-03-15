// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_DA_SettingsCollection.h
 *
 * DataAsset grouping UMCore_DA_SettingDefinition assets into a single
 * reference that settings widgets and the library can consume.
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MCore_DA_SettingsCollection.generated.h"

class UMCore_DA_SettingDefinition;

/**
 * Collection of setting definitions for a settings panel.
 *
 * Key Features:
 * - Category-based filtering with sort-order awareness
 * - Tag-based setting lookup
 * - Editor data validation for null entries and duplicate tags
 *
 * Blueprint Usage:
 *   Create as a DataAsset. Assign to UMCore_CoreSettings::DefaultSettingsCollection
 *   or pass directly to settings UI widgets.
 */
UCLASS(BlueprintType, Const)
class MODULUSCORE_API UMCore_DA_SettingsCollection : public UDataAsset
{
	GENERATED_BODY()

public:
	// ============================================================================
	// PROPERTIES
	// ============================================================================

	/* Display name for this collection (e.g. "Default Settings", "PC Settings") */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collection")
	FText CollectionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collection")
	TArray<TObjectPtr<UMCore_DA_SettingDefinition>> Settings;

	// ============================================================================
	// METHODS
	// ============================================================================

	/**
	 * Get all settings matching a category tag, sorted by SortOrder.
	 *
	 * @param CategoryTag  The category to filter by.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	TArray<UMCore_DA_SettingDefinition*> GetSettingsInCategory(const FGameplayTag& CategoryTag) const;

	/**
	 * Get all unique category tags present in this collection,
	 * sorted by lowest SortOrder in each category.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	TArray<FGameplayTag> GetAllCategories() const;

	const TArray<TObjectPtr<UMCore_DA_SettingDefinition>>& GetAllSettings() const { return Settings; }

	/**
	 * Find a single setting by its tag.
	 *
	 * @param SettingTag  The gameplay tag to search for.
	 * @return  The matching definition, or nullptr if not found.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	UMCore_DA_SettingDefinition* FindSettingByTag(const FGameplayTag& SettingTag) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	int32 GetSettingCount() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};
