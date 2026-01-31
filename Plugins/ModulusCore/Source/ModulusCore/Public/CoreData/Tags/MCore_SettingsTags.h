// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "NativeGameplayTags.h"

enum class EMCore_SettingType : uint8;

namespace MCore_SettingsTags
{
    /** Setting Categories */
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Category_Display);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Category_Graphics);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Category_Audio);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Category_Controls);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Category_Accessibility);

    /** Display Settings */
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Display_Resolution);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Display_WindowMode);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Display_VSync);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Display_FrameRateLimit);

    /** Graphics Settings */
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Graphics_QualityPreset);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Graphics_TextureQuality);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Graphics_ShadowQuality);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Graphics_AntiAliasing);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Graphics_ViewDistance);

    /** Audio Settings */
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Audio_MasterVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Audio_MusicVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Audio_SFXVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Audio_VoiceVolume);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Audio_Quality);

    /** Controls Settings */
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Controls_MouseSensitivity);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Controls_InvertY);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Controls_GamepadSensitivity);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Controls_MoveForward);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Controls_Jump);

    /** Accessibility Settings */
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Accessibility_Subtitles);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Accessibility_SubtitleSize);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Accessibility_ColorblindMode);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Accessibility_UIScale);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Accessibility_ReduceMotion);

    /** Input Settings */
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_SensitivityX);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_SensitivityY);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_InvertLookX);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_InvertLookY);

    /** Utility Functions */
    MODULUSCORE_API FGameplayTag GetWidgetTypeTag(EMCore_SettingType SettingType);
    MODULUSCORE_API FGameplayTag GetCategoryTag(const FString& CategoryName);
    MODULUSCORE_API FGameplayTag GenerateCustomUISettingTag(const FString& Category, const FString& Setting);
}