// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_SettingsTags.h
 *
 * Native gameplay tag declarations for the settings system.
 * Covers categories, sub-categories, individual settings, and events.
 */

#pragma once

#include "NativeGameplayTags.h"

enum class EMCore_SettingType : uint8;

namespace MCore_SettingsTags
{
    // ========================================================================
    // SETTING CATEGORIES
    // ========================================================================

    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Display);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Graphics);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Audio);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Controls);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Accessibility);

    // ========================================================================
    // DEFAULT SUB-CATEGORIES
    // ========================================================================

    /*
     * Depth-4 tags (Category.X.General) for settings panel tab layout.
     * Single depth-4 child named General = no sub-tab bar.
     * Multiple depth-4 children under the same parent = sub-tabs.
     */
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Display_General);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Graphics_General);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Audio_General);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Controls_General);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Accessibility_General);

    // ========================================================================
    // DISPLAY SETTINGS
    // ========================================================================

    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Display_Brightness);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Display_Resolution);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Display_WindowMode);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Display_VSync);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Display_FrameRateLimit);

    // ========================================================================
    // GRAPHICS SETTINGS
    // ========================================================================

    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Graphics_QualityPreset);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Graphics_TextureQuality);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Graphics_FoliageQuality);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Graphics_PostProcessing);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Graphics_ShadowQuality);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Graphics_AntiAliasing);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Graphics_ViewDistance);

    // ========================================================================
    // AUDIO SETTINGS
    // ========================================================================

    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_MasterVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_Quality);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_MusicVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_SFXVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_DialogueVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_AmbientVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_UIVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_MuteAudio);

    // ========================================================================
    // CONTROLS SETTINGS
    // ========================================================================

    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_MouseSensitivity);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_MouseSmoothing);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_GamepadSensitivity);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_SensitivityX);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_SensitivityY);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_InvertLookX);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_InvertLookY);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_Vibration);

    // ========================================================================
    // ACCESSIBILITY SETTINGS
    // ========================================================================

    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_Subtitles);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_SubtitleSize);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_UIScale);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_ColorblindMode);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_ColorblindStrength);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_TooltipDelay);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_ScreenReader);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_LargeText);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_UITextSize);

    // ========================================================================
    // SETTING EVENTS
    // ========================================================================

    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Event_ConfirmationRequired);

    // ========================================================================
    // UTILITY FUNCTIONS
    // ========================================================================

    MODULUSCORE_API FGameplayTag GetWidgetTypeTag(EMCore_SettingType SettingType);
    MODULUSCORE_API FGameplayTag GetCategoryTag(const FString& CategoryName);
    MODULUSCORE_API FGameplayTag GenerateCustomUISettingTag(const FString& Category, const FString& Setting);
}
