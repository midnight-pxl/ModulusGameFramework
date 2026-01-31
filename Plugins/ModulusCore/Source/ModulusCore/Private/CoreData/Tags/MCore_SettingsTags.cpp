// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreData/Tags/MCore_SettingsTags.h"

#include "CoreData/Types/Settings/MCore_GameSettingType.h" /** For EMCore_SettingType */

namespace MCore_SettingsTags
{
    
    /** Setting Categories */
    UE_DEFINE_GAMEPLAY_TAG(Category_Display, "Category.Display");
    UE_DEFINE_GAMEPLAY_TAG(Category_Graphics, "Category.Graphics");
    UE_DEFINE_GAMEPLAY_TAG(Category_Audio, "Category.Audio");
    UE_DEFINE_GAMEPLAY_TAG(Category_Controls, "Category.Controls");
    UE_DEFINE_GAMEPLAY_TAG(Category_Accessibility, "Category.Accessibility");

    /** Display Settings */
    UE_DEFINE_GAMEPLAY_TAG(Display_Resolution, "Display.Resolution");
    UE_DEFINE_GAMEPLAY_TAG(Display_WindowMode, "Display.WindowMode");
    UE_DEFINE_GAMEPLAY_TAG(Display_VSync, "Display.VSync");
    UE_DEFINE_GAMEPLAY_TAG(Display_FrameRateLimit, "Display.FrameRateLimit");

    /** Graphics Settings */
    UE_DEFINE_GAMEPLAY_TAG(Graphics_QualityPreset, "Graphics.QualityPreset");
    UE_DEFINE_GAMEPLAY_TAG(Graphics_TextureQuality, "Graphics.TextureQuality");
    UE_DEFINE_GAMEPLAY_TAG(Graphics_ShadowQuality, "Graphics.ShadowQuality");
    UE_DEFINE_GAMEPLAY_TAG(Graphics_AntiAliasing, "Graphics.AntiAliasing");
    UE_DEFINE_GAMEPLAY_TAG(Graphics_ViewDistance, "Graphics.ViewDistance");

    /** Audio Settings */
    UE_DEFINE_GAMEPLAY_TAG(Audio_MasterVolume, "Audio.MasterVolume");
    UE_DEFINE_GAMEPLAY_TAG(Audio_MusicVolume, "Audio.MusicVolume");
    UE_DEFINE_GAMEPLAY_TAG(Audio_SFXVolume, "Audio.SFXVolume");
    UE_DEFINE_GAMEPLAY_TAG(Audio_VoiceVolume, "Audio.VoiceVolume");
    UE_DEFINE_GAMEPLAY_TAG(Audio_Quality, "Audio.Quality");

    /** Controls Settings */
    UE_DEFINE_GAMEPLAY_TAG(Controls_MouseSensitivity, "Controls.MouseSensitivity");
    UE_DEFINE_GAMEPLAY_TAG(Controls_InvertY, "Controls.InvertY");
    UE_DEFINE_GAMEPLAY_TAG(Controls_GamepadSensitivity, "Controls.GamepadSensitivity");
    UE_DEFINE_GAMEPLAY_TAG(Controls_MoveForward, "Controls.MoveForward");
    UE_DEFINE_GAMEPLAY_TAG(Controls_Jump, "Controls.Jump");

    /** Accessibility Settings */
    UE_DEFINE_GAMEPLAY_TAG(Accessibility_Subtitles, "Accessibility.Subtitles");
    UE_DEFINE_GAMEPLAY_TAG(Accessibility_SubtitleSize, "Accessibility.SubtitleSize");
    UE_DEFINE_GAMEPLAY_TAG(Accessibility_ColorblindMode, "Accessibility.ColorblindMode");
    UE_DEFINE_GAMEPLAY_TAG(Accessibility_UIScale, "Accessibility.UIScale");
    UE_DEFINE_GAMEPLAY_TAG(Accessibility_ReduceMotion, "Accessibility.ReduceMotion");

    /** Input Settings */
    UE_DEFINE_GAMEPLAY_TAG(Input_SensitivityX, "Input.SensitivityX");
    UE_DEFINE_GAMEPLAY_TAG(Input_SensitivityY, "Input.SensitivityY");
    UE_DEFINE_GAMEPLAY_TAG(Input_InvertLookX, "Input.InvertLookX");
    UE_DEFINE_GAMEPLAY_TAG(Input_InvertLookY, "Input.InvertLookY");

    /** Utility Functions */
    FGameplayTag GetCategoryTag(const FString& CategoryName)
    {
        if (CategoryName.Equals(TEXT("Display"), ESearchCase::IgnoreCase))
            return Category_Display;
        if (CategoryName.Equals(TEXT("Graphics"), ESearchCase::IgnoreCase))
            return Category_Graphics;
        if (CategoryName.Equals(TEXT("Audio"), ESearchCase::IgnoreCase))
            return Category_Audio;
        if (CategoryName.Equals(TEXT("Controls"), ESearchCase::IgnoreCase))
            return Category_Controls;
        if (CategoryName.Equals(TEXT("Accessibility"), ESearchCase::IgnoreCase))
            return Category_Accessibility;
        
        return FGameplayTag::EmptyTag;
    }

    FGameplayTag GenerateCustomUISettingTag(const FString& Category, const FString& UISetting)
    {
        FString TagString = FString::Printf(TEXT("%s.%s"), *Category, *UISetting);
        return FGameplayTag::RequestGameplayTag(FName(*TagString));
    }
}
