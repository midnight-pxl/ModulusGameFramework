// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "NativeGameplayTags.h"

enum class EMCore_SettingType : uint8;

namespace MCore_SettingsTags
{
    /** Setting Categories */
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Display);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Graphics);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Audio);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Controls);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Category_Accessibility);

    /** Display Settings */
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Display_Brightness);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Display_Resolution);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Display_WindowMode);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Display_VSync);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Display_FrameRateLimit);

    /** Graphics Settings */
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Graphics_QualityPreset);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Graphics_TextureQuality);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Graphics_FoliageQuality);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Graphics_PostProcessing);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Graphics_ShadowQuality);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Graphics_AntiAliasing);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Graphics_ViewDistance);

    /** Audio Settings */
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_MasterVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_Quality);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_MusicVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_SFXVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_VoiceVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_AmbientVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_UIVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Audio_MuteAudio);

    /** Controls Settings */
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_MouseSensitivity);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_MouseSmoothing);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_GamepadSensitivity);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_SensitivityX);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_SensitivityY);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_InvertLookX);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_InvertLookY);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Controls_Vibration);

    /** Accessibility Settings */
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_Subtitles);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_SubtitleSize);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_UIScale);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_ColorblindMode);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_ColorblindStrength);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_TooltipDelay);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_ScreenReader);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_Settings_Accessibility_LargeText);

    /** Utility Functions */
    MODULUSCORE_API FGameplayTag GetWidgetTypeTag(EMCore_SettingType SettingType);
    MODULUSCORE_API FGameplayTag GetCategoryTag(const FString& CategoryName);
    MODULUSCORE_API FGameplayTag GenerateCustomUISettingTag(const FString& Category, const FString& Setting);
}