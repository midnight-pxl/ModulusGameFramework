// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InputCoreTypes.h"
#include "MCore_SettingsTypes.h"
#include "MCore_DA_SettingDefinition.generated.h"

class UInputAction;
class UInputMappingContext;
class USoundClass;

/**
 * Data Asset for defining a single game setting.
 *
 * Each asset describes one setting: type, value range, defaults,
 * category, how it applies to engine systems (UGameUserSettings, CVars,
 * SoundClasses), etc
 */
UCLASS(BlueprintType, Const)
class MODULUSCORE_API UMCore_DA_SettingDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	/** Unique gameplay tag identifying this setting (e.g. Settings.Graphics.ShadowQuality) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Identity",
		meta = (Categories = "Settings"))
	FGameplayTag SettingTag;

	/** Display name shown in settings UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Identity")
	FText DisplayName;

	/** Optional tooltip/description */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Identity")
	FText Description;
	
	// ========================================================================
	// TYPE & VALUES
	// ========================================================================

	/** What kind of control this setting uses */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Type")
	EMCore_SettingType SettingType = EMCore_SettingType::Toggle;

	/** Minimum value (Slider type) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Slider",
		meta = (EditCondition = "SettingType == EMCore_SettingType::Slider",
				EditConditionHides, ClampMin = "0.0"))
	float MinValue{0.0f};

	/** Maximum value (Slider type) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Slider",
		meta = (EditCondition = "SettingType == EMCore_SettingType::Slider",
				EditConditionHides, ClampMin = "0.0"))
	float MaxValue{1.0f};

	/** Step increment (Slider type) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Slider",
		meta = (EditCondition = "SettingType == EMCore_SettingType::Slider",
				EditConditionHides))
	float StepSize{0.02f};

	/** Default float value (Slider type) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Slider",
		meta = (EditCondition = "SettingType == EMCore_SettingType::Slider",
				EditConditionHides))
	float DefaultValue{0.5f};

	/** Default bool value (Toggle type) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Toggle",
		meta = (EditCondition = "SettingType == EMCore_SettingType::Toggle",
				EditConditionHides))
	bool DefaultToggleValue{false};

	/** Dropdown option labels (Dropdown type) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Dropdown",
		meta = (EditCondition = "SettingType == EMCore_SettingType::Dropdown",
				EditConditionHides))
	TArray<FText> DropdownOptions;

	/** Default selected index (Dropdown type) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Dropdown",
		meta = (EditCondition = "SettingType == EMCore_SettingType::Dropdown",
				EditConditionHides, ClampMin = "0"))
	int32 DefaultDropdownIndex{0};

	/** Default text (TextInput type) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|TextInput",
		meta = (EditCondition = "SettingType == EMCore_SettingType::TextInput",
		        EditConditionHides))
	FString DefaultTextValue;

	/** Max character length, 0 = unlimited (TextInput type) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|TextInput",
		meta = (EditCondition = "SettingType == EMCore_SettingType::TextInput",
		        EditConditionHides, ClampMin = "0"))
	int32 MaxTextLength{100};

	/** Default key binding (KeyBinding type) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|KeyBinding",
		meta = (EditCondition = "SettingType == EMCore_SettingType::KeyBinding",
		        EditConditionHides))
	FKey DefaultKey;

	/** Input action for key binding discovery (KeyBinding type) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|KeyBinding",
		meta = (EditCondition = "SettingType == EMCore_SettingType::KeyBinding",
		        EditConditionHides))
	TObjectPtr<const UInputAction> InputAction;

	/** Input mapping context (KeyBinding type) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|KeyBinding",
		meta = (EditCondition = "SettingType == EMCore_SettingType::KeyBinding",
		        EditConditionHides))
	TObjectPtr<const UInputMappingContext> InputMappingContext;

	/** Button label text (Action type) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Action",
		meta = (EditCondition = "SettingType == EMCore_SettingType::Action",
		        EditConditionHides))
	FText ActionButtonText = FText::FromString("Execute");

	// ========================================================================
	// CATEGORIZATION
	// ========================================================================

	/** Category tag for grouping (e.g. Settings.Category.Graphics) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Category",
		meta = (Categories = "Settings.Category"))
	FGameplayTag CategoryTag;

	/** Sort priority within category (lower = appears first) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Category")
	int32 SortOrder{0};

	// ========================================================================
	// APPLY CONFIGURATION
	// ========================================================================

	/** UGameUserSettings property name to write to (empty = custom handling only) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Apply",
		meta = (DisplayName = "GameUserSettings Property"))
	FName GameUserSettingsProperty;

	/** Console variable to write to (empty = none) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Apply")
	FName ConsoleVariable;

	/** Sound class to adjust volume on (Audio settings) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Apply",
		meta = (EditCondition = "SettingType == EMCore_SettingType::Slider",
		        EditConditionHides))
	TSoftObjectPtr<USoundClass> SoundClass;

	// ========================================================================
	// METHODS
	// ========================================================================

	/** Get the save key derived from SettingTag (dots replaced with underscores) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	FString GetSaveKey() const;

	/** Validate this setting definition */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModulusCore|Settings")
	bool IsValid() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};
