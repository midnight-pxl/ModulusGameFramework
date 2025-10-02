// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonInputBaseTypes.h"
#include "MCore_GameSettingType.h"
#include "CoreData/CoreGameplayTags/MCore_UISettingsTags.h"
#include "CoreData/CoreDevSettings/MCore_CommonUISettings.h"
#include "MCore_GameSettingTemplates.generated.h"

enum class EPlayerMappableKeySlot : uint8;
class UInputMappingContext;
class UInputAction;

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
	
	UFUNCTION(BlueprintCallable, Category = "Settings|Enhanced Input")
	static void GetMappableActionsFromContext(
		const UInputMappingContext* Context,
		ECommonInputType InputType,
		TArray<UInputAction*>& OutActions);

	UFUNCTION(BlueprintCallable, Category = "Settings|Enhanced Input")
	static TArray<FMCore_KeyBindingCategory> GetAllKeyBindingCategories(
		UObject* WorldContext, 
		ECommonInputType InputType = ECommonInputType::MouseAndKeyboard);

	UFUNCTION(BlueprintPure, Category = "Settings|Enhanced Input")
	static FName GetActionDisplayCategory(const UInputAction* Action);

	UFUNCTION(BlueprintCallable, Category = "Settings|Enhanced Input")
	static TArray<FName> GetActionsUsingKey(
		FKey Key,
		FName ExcludeAction = NAME_None);

	UFUNCTION(BlueprintPure, Category = "Settings|Enhanced Input")
	static FKey GetCurrentKeyForAction(
		const UInputAction* Action,
		EPlayerMappableKeySlot Slot);
	
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

	static FMCore_SettingDefinition CreateSliderSetting(
		const FGameplayTag& SettingTag,
		const FText& DisplayName,
		const FText& Description,
		float MinValue, float MaxValue,
		float DefaultValue,
		float StepValue);

	static FMCore_SettingDefinition CreateDropdownSetting(
		const FGameplayTag& SettingTag,
		const FText& DisplayName,
		const FText& Description,
		const TArray<FText>& Options,
		int32 DefaultIndex);
	
	static FMCore_SettingDefinition CreateToggleSetting(
		const FGameplayTag& SettingTag,
		const FText& DisplayName,
		const FText& Description,
		bool DefaultValue);

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
