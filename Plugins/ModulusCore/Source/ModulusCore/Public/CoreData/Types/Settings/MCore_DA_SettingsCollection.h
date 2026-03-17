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
 * Provides category-based filtering, tag-based lookup, and sort-order awareness.
 *
 * Create as a DataAsset; assign to UMCore_CoreSettings::DefaultSettingsCollection.
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collection")
	TMap<FGameplayTag, FText> CategoryDisplayName;

	// ============================================================================
	// METHODS
	// ============================================================================

	/** Returns all settings matching a category tag, sorted by SortOrder. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	TArray<UMCore_DA_SettingDefinition*> GetSettingsInCategory(const FGameplayTag& CategoryTag) const;

	/** Returns all unique category tags, sorted by lowest SortOrder in each category. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	TArray<FGameplayTag> GetAllCategories() const;

	const TArray<TObjectPtr<UMCore_DA_SettingDefinition>>& GetAllSettings() const { return Settings; }

	/** Find a single setting by its tag. Returns nullptr if not found. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	UMCore_DA_SettingDefinition* FindSettingByTag(const FGameplayTag& SettingTag) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	int32 GetSettingCount() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};
