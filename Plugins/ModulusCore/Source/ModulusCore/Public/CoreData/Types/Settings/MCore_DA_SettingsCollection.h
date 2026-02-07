// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MCore_DA_SettingsCollection.generated.h"

class UMCore_DA_SettingDefinition;

/**
 * Collection of setting definitions for a settings panel.
 *
 * Groups UMCore_SettingDefinition assets into a single reference that
 * a settings widget can consume.
 */
UCLASS(BlueprintType, Const)
class MODULUSCORE_API UMCore_DA_SettingsCollection : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// ========================================================================
	// PROPERTIES
	// ========================================================================

	/** Display name for this collection (e.g. "Default Settings", "PC Settings") */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collection")
	FText CollectionName;

	/** Ordered list of setting definitions in this collection */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collection")
	TArray<TObjectPtr<UMCore_DA_SettingDefinition>> Settings;

	// ========================================================================
	// METHODS
	// ========================================================================

	/** Get all settings matching a category tag */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	TArray<UMCore_DA_SettingDefinition*> GetSettingsInCategory(const FGameplayTag& CategoryTag) const;

	/** Get all unique category tags present in this collection, sorted by lowest SortOrder in each category */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	TArray<FGameplayTag> GetAllCategories() const;

	/** Find a single setting by its tag. Returns nullptr if not found. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	UMCore_DA_SettingDefinition* FindSettingByTag(const FGameplayTag& SettingTag) const;

	/** Total number of valid settings in this collection */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	int32 GetSettingCount() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};
