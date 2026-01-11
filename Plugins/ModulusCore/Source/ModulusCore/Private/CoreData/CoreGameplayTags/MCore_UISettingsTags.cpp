// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreData/CoreGameplayTags/MCore_UISettingsTags.h"

#include "CoreData/CoreStructEnums/SettingsStructsEnums/CoreGameSettings/MCore_GameSettingType.h" /** For EMCore_SettingType */

namespace MCore_UISettingsTags
{
    /** Setting Categories */
    UE_DEFINE_GAMEPLAY_TAG(UI_Categories_Display, "UI.Categories.Display");
    UE_DEFINE_GAMEPLAY_TAG(UI_Categories_Graphics, "UI.Categories.Graphics");
    UE_DEFINE_GAMEPLAY_TAG(UI_Categories_Audio, "UI.Categories.Audio");
    UE_DEFINE_GAMEPLAY_TAG(UI_Categories_Controls, "UI.Categories.Controls");
    UE_DEFINE_GAMEPLAY_TAG(UI_Categories_Accessibility, "UI.Categories.Accessibility");

    /** Display Settings */
    UE_DEFINE_GAMEPLAY_TAG(Settings_Display_Resolution, "Settings.Display.Resolution");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Display_WindowMode, "Settings.Display.WindowMode");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Display_VSync, "Settings.Display.VSync");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Display_FrameRateLimit, "Settings.Display.FrameRateLimit");

    /** Graphics Settings */
    UE_DEFINE_GAMEPLAY_TAG(Settings_Graphics_QualityPreset, "Settings.Graphics.QualityPreset");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Graphics_TextureQuality, "Settings.Graphics.TextureQuality");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Graphics_ShadowQuality, "Settings.Graphics.ShadowQuality");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Graphics_AntiAliasing, "Settings.Graphics.AntiAliasing");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Graphics_ViewDistance, "Settings.Graphics.ViewDistance");

    /** Audio Settings */
    UE_DEFINE_GAMEPLAY_TAG(Settings_Audio_MasterVolume, "Settings.Audio.MasterVolume");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Audio_MusicVolume, "Settings.Audio.MusicVolume");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Audio_SFXVolume, "Settings.Audio.SFXVolume");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Audio_VoiceVolume, "Settings.Audio.VoiceVolume");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Audio_Quality, "Settings.Audio.Quality");

    /** Controls Settings */
    UE_DEFINE_GAMEPLAY_TAG(Settings_Controls_MouseSensitivity, "Settings.Controls.MouseSensitivity");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Controls_InvertY, "Settings.Controls.InvertY");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Controls_GamepadSensitivity, "Settings.Controls.GamepadSensitivity");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Controls_MoveForward, "Settings.Controls.MoveForward");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Controls_Jump, "Settings.Controls.Jump");

    /** Accessibility Settings */
    UE_DEFINE_GAMEPLAY_TAG(Settings_Accessibility_Subtitles, "Settings.Accessibility.Subtitles");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Accessibility_SubtitleSize, "Settings.Accessibility.SubtitleSize");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Accessibility_ColorblindMode, "Settings.Accessibility.ColorblindMode");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Accessibility_UIScale, "Settings.Accessibility.UIScale");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Accessibility_ReduceMotion, "Settings.Accessibility.ReduceMotion");

    /** Input Settings */
    UE_DEFINE_GAMEPLAY_TAG(Settings_Input_SensitivityX, "Settings.Input.SensitivityX");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Input_SensitivityY, "Settings.Input.SensitivityY");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Input_InvertLookX, "Settings.Input.InvertLookX");
    UE_DEFINE_GAMEPLAY_TAG(Settings_Input_InvertLookY, "Settings.Input.InvertLookY");

    /** Utility Functions */
    FGameplayTag GetCategoryTag(const FString& CategoryName)
    {
        if (CategoryName.Equals(TEXT("Display"), ESearchCase::IgnoreCase))
            return UI_Categories_Display;
        if (CategoryName.Equals(TEXT("Graphics"), ESearchCase::IgnoreCase))
            return UI_Categories_Graphics;
        if (CategoryName.Equals(TEXT("Audio"), ESearchCase::IgnoreCase))
            return UI_Categories_Audio;
        if (CategoryName.Equals(TEXT("Controls"), ESearchCase::IgnoreCase))
            return UI_Categories_Controls;
        if (CategoryName.Equals(TEXT("Accessibility"), ESearchCase::IgnoreCase))
            return UI_Categories_Accessibility;
        
        return FGameplayTag::EmptyTag;
    }

    FGameplayTag GenerateCustomUISettingTag(const FString& Category, const FString& UISetting)
    {
        FString TagString = FString::Printf(TEXT("Settings.%s.%s"), *Category, *UISetting);
        return FGameplayTag::RequestGameplayTag(FName(*TagString));
    }
}
