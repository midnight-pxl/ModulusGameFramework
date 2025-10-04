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
 * Game setting template factory
 *
 * Pre-configured setting categories for common game settings (Display, Graphics, Audio, Controls, Accessibility).
 * Use these templates as starting points, then customize for your game.
 *
 * Blueprint Usage:
 * 1. Call CreateGraphicsSettings(), CreateAudioSettings(), etc. to get pre-configured categories
 * 2. Modify returned FMCore_SettingCategory as needed (add/remove settings, change defaults)
 * 3. Pass to settings menu for display
 *
 * Enhanced Input:
 * - GetAllKeyBindingCategories() discovers all player-mappable actions from registered contexts
 * - GetActionsUsingKey() detects key binding conflicts
 * - GetCurrentKeyForAction() retrieves player's current key binding
 */
UCLASS(BlueprintType)
class MODULUSCORE_API UMCore_GameSettingTemplates : public UObject
{
	GENERATED_BODY()

public:

	/** Create Display settings category (Resolution, Window Mode, VSync, Frame Rate Limit) */
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Settings|Templates")
    static FMCore_SettingCategory CreateDisplaySettings();

	/** Create Graphics settings category (Quality Preset, Textures, Shadows, Anti-Aliasing, View Distance) */
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Settings|Templates")
    static FMCore_SettingCategory CreateGraphicsSettings();

	/** Create Audio settings category (Master, Music, SFX, Voice volumes, Audio Quality) */
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Settings|Templates")
    static FMCore_SettingCategory CreateAudioSettings();

	/** Create Controls settings category (Mouse Sensitivity, Invert Y, Gamepad Sensitivity, Key Bindings) */
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Settings|Templates")
    static FMCore_SettingCategory CreateControlsSettings();

	/** Create Accessibility settings category (Subtitles, UI Scale, Colorblind Mode, Reduce Motion) */
    UFUNCTION(BlueprintCallable, CallInEditor, Category="Settings|Templates")
    static FMCore_SettingCategory CreateAccessibilitySettings();

	/**
	 * Get player-mappable actions from an Input Mapping Context
	 *
	 * Filters actions by input type (keyboard/mouse vs gamepad).
	 * Only returns actions marked as "Player Mappable" in their settings.
	 *
	 * @param Context - Input Mapping Context to scan
	 * @param InputType - Filter by KeyboardAndMouse or Gamepad
	 * @param OutActions - Array of player-mappable Input Actions
	 */
	UFUNCTION(BlueprintCallable, Category = "Settings|Enhanced Input")
	static void GetMappableActionsFromContext(
		const UInputMappingContext* Context,
		ECommonInputType InputType,
		TArray<UInputAction*>& OutActions);

	/**
	 * Get all key binding categories from registered Input Mapping Contexts
	 *
	 * Discovers player-mappable actions from Enhanced Input User Settings.
	 * Groups actions by their Display Category for organized settings menus.
	 *
	 * @param InputType - Filter by KeyboardAndMouse or Gamepad bindings
	 * @return Array of key binding categories (Movement, Combat, Inventory, etc.)
	 */
	UFUNCTION(BlueprintCallable, Category = "Settings|Enhanced Input")
	static TArray<FMCore_KeyBindingCategory> GetAllKeyBindingCategories(
		UObject* WorldContext,
		ECommonInputType InputType = ECommonInputType::MouseAndKeyboard);

	/** Get the display category for an Input Action (for grouping in settings UI) */
	UFUNCTION(BlueprintPure, Category = "Settings|Enhanced Input")
	static FName GetActionDisplayCategory(const UInputAction* Action);

	/**
	 * Find all actions currently using a specific key (for conflict detection)
	 *
	 * @param Key - Key to check (e.g., F, Space, LeftMouseButton)
	 * @param ExcludeAction - Action to exclude from results (usually the one being rebound)
	 * @return Action names using this key
	 */
	UFUNCTION(BlueprintCallable, Category = "Settings|Enhanced Input")
	static TArray<FName> GetActionsUsingKey(
		FKey Key,
		FName ExcludeAction = NAME_None);

	/** Get the current key binding for an Input Action */
	UFUNCTION(BlueprintPure, Category = "Settings|Enhanced Input")
	static FKey GetCurrentKeyForAction(
		const UInputAction* Action,
		EPlayerMappableKeySlot Slot);

	/** Create an empty setting category (for custom settings) */
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
