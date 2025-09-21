// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MCore_GameSettingType.h"
#include "CoreUISystem/MCore_UISettingsTags.h"
#include "CoreData/CoreDevSettings/MCore_CommonUISettings.h"
#include "MCore_GameSettingTemplates.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MODULUSCORE_API UMCore_GameSettingTemplates : public UObject
{
	GENERATED_BODY()

	public:

    UFUNCTION(BlueprintCallable, CallInEditor, Category="Settings|Templates")
    static FMCore_SettingCategory CreateDisplaySettings();

    UFUNCTION(BlueprintCallable, CallInEditor, Category="Settings|Templates")
    static FMCore_SettingCategory CreateGraphicsSettings();

    UFUNCTION(BlueprintCallable, CallInEditor, Category="Settings|Templates")
    static FMCore_SettingCategory CreateAudioSettings();
    
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Settings|Templates")
    static FMCore_SettingCategory CreateControlsSettings();

    UFUNCTION(BlueprintCallable, CallInEditor, Category="Settings|Templates")
    static FMCore_SettingCategory CreateAccessibilitySettings();
	
    /** Config Helpers
	 * Creates complete settings configuration with all 5 universal categories.
	 * This is the main function for "15-minute setup"
	 */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Modulus Settings|Templates")
	static FMCore_SettingsConfiguration CreateCompleteSettingsConfiguration();

	/**
	 * Creates minimal settings configuration (Display + Audio only) for simple games.
	 */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Modulus Settings|Templates")
	static FMCore_SettingsConfiguration CreateMinimalSettingsConfiguration();

	
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "Modulus Settings|Templates")
    static FMCore_SettingCategory CreateEmptyCategory(const FString& CategoryName, const FString& Description = "")
    {
        FMCore_SettingCategory Category;
        Category.CategoryName = FText::FromString(CategoryName);
        Category.CategoryDescription = FText::FromString(Description.IsEmpty() ? CategoryName + " settings" : Description);
        Category.CategoryTag = MCore_UISettingsTags::GenerateCustomUISettingTag("Category", CategoryName);
        return Category;
    }

private:
	static TArray<FMCore_SettingDefinition> CreateSliders();
	static TArray<FMCore_SettingDefinition> CreateDropdowns();
	static TArray<FMCore_SettingDefinition> CreateToggles();

	static FMCore_SettingDefinition CreateSliderSetting(
		const FGameplayTag& SaveKey,
		const FText& DisplayName,
		const FText& Description,
		float MinValue, float MaxValue,
		float DefaultValue,
		float StepValue);

	static FMCore_SettingDefinition CreateDropdownSetting(
	const FGameplayTag& SaveKey,
	const FText& DisplayName,
	const FText& Description,
	const TArray<FText>& Options,
	int32 DefaultIndex);
	
	static FMCore_SettingDefinition CreateToggleSetting(
	const FGameplayTag& SaveKey,
	const FText& DisplayName,
	const FText& Description,
	float DefaultValue);

	/**
	 * Helper to create a key binding setting from Enhanced Input Action metadata.
	 * Uses action's display name, description, and discovers default key binding.
	 */
	static FMCore_SettingDefinition CreateKeyBindingSetting(
		const UInputAction* InputAction,
		const class UInputMappingContext* MappingContext);

	/**
	 * Helper to create sensitivity and toggle settings for basic controls.
	 * These are the non-keybinding control settings (mouse sensitivity, invert Y, etc.)
	 */
	static TArray<FMCore_SettingDefinition> CreateBasicControlDefinition();
	
	static bool ValidateEnhancedInputSetup(UObject* WorldContext, TArray<FString>& ValidationErrors);
};
