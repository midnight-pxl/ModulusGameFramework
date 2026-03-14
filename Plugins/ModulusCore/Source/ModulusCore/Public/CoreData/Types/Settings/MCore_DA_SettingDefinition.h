// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MCore_SettingsTypes.h"
#include "MCore_DA_SettingDefinition.generated.h"

class UInputAction;
class UInputMappingContext;
class USoundClass;

/** How slider values display in the UI */
UENUM(BlueprintType)
enum class EMCore_SliderDisplayFormat : uint8
{
	/** Show raw value: 0.75 */
	RawValue,
	/** Show percentage: 75% */
	Percentage,
	/** Show integer: 75 */
	WholeNumber
};

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
	
	/** How this slider value displays in the UI (Slider type) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Slider",
		meta = (EditCondition = "SettingType == EMCore_SettingType::Slider",
				EditConditionHides))
	EMCore_SliderDisplayFormat SliderDisplayFormat = EMCore_SliderDisplayFormat::RawValue;

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

	/**
	 * Optional visual grouping label within a settings page.
	 * Settings sharing the same SectionName under a sub-category are rendered
	 * under a shared section header. Empty means no section header.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Category")
	FText SectionName;

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
	// BEHAVIOR
	// ========================================================================

	/**
	 * If true, applying this setting immediately shows UMCore_SettingsRevertCountdown.
	 * Use for destructive settings (resolution, display mode) that may render
	 * the screen unreadable. The countdown auto-reverts if not confirmed.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Behavior",
		meta = (DisplayName = "Requires Confirmation"))
	bool bRequiresConfirmation = false;

	/**
	 * Duration of the revert countdown in seconds.
	 * Only relevant when bRequiresConfirmation is true.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting|Behavior",
		meta = (EditCondition = "bRequiresConfirmation",
				EditConditionHides,
				ClampMin = "5.0", ClampMax = "30.0",
				DisplayName = "Confirmation Revert Delay (s)"))
	float ConfirmationRevertDelay = 10.0f;

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
