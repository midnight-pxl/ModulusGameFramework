// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "NativeGameplayTags.h"

enum class EMCore_SettingType : uint8;

namespace MCore_UISettingsTags
{
    // Setting Categories
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Categories_Display);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Categories_Graphics);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Categories_Audio);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Categories_Controls);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Categories_Accessibility);

    // Display Settings
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Display_Resolution);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Display_WindowMode);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Display_VSync);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Display_FrameRateLimit);

    // Graphics Settings
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Graphics_QualityPreset);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Graphics_TextureQuality);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Graphics_ShadowQuality);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Graphics_AntiAliasing);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Graphics_ViewDistance);

    // Audio Settings  
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Audio_MasterVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Audio_MusicVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Audio_SFXVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Audio_VoiceVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Audio_Quality);

    // Controls Settings
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Controls_MouseSensitivity);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Controls_InvertY);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Controls_GamepadSensitivity);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Controls_MoveForward);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Controls_Jump);

    // Accessibility Settings
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Accessibility_Subtitles);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Accessibility_SubtitleSize);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Accessibility_ColorblindMode);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Accessibility_UIScale);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Accessibility_ReduceMotion);

    // Widget Pool Management
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Widget_Toggle);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Widget_Slider);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Widget_Dropdown);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Widget_TextInput);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Widget_KeyBinding);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Settings_Widget_Action);

    // Utility Functions
    MODULUSCORE_API FGameplayTag GetWidgetTypeTag(EMCore_SettingType SettingType);

    MODULUSCORE_API FGameplayTag GetCategoryTag(const FString& CategoryName);

    MODULUSCORE_API FGameplayTag GenerateCustomUISettingTag(const FString& Category, const FString& UISetting);
}