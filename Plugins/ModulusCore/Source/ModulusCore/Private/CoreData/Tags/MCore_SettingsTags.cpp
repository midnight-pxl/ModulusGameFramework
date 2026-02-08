// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreData/Tags/MCore_SettingsTags.h"

#include "CoreData/Types/Settings/MCore_GameSettingType.h" /** For EMCore_SettingType */

namespace MCore_SettingsTags
{
    
    /** Setting Categories */
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Category_Display, "MCore.Settings.Category.Display");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Category_Graphics, "MCore.Settings.Category.Graphics");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Category_Audio, "MCore.Settings.Category.Audio");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Category_Controls, "MCore.Settings.Category.Controls");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Category_Accessibility, "MCore.Settings.Category.Accessibility");

    /** Display Settings */
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Display_Brightness, "MCore.Settings.Display.Brightness");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Display_Resolution, "MCore.Settings.Display.Resolution");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Display_WindowMode, "MCore.Settings.Display.WindowMode");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Display_VSync, "MCore.Settings.Display.VSync");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Display_FrameRateLimit, "Display.FrameRateLimit");

    /** Graphics Settings */
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Graphics_QualityPreset, "MCore.Settings.Graphics.QualityPreset");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Graphics_TextureQuality, "MCore.Settings.Graphics.TextureQuality");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Graphics_FoliageQuality, "MCore.Settings.Graphics.FoliageQuality");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Graphics_PostProcessing, "MCore.Settings.Graphics.PostProcessing");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Graphics_ShadowQuality, "MCore.Settings.Graphics.ShadowQuality");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Graphics_AntiAliasing, "MCore.Settings.Graphics.AntiAliasing");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Graphics_ViewDistance, "MCore.Settings.Graphics.ViewDistance");

    /** Audio Settings */
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Audio_MasterVolume, "MCore.Settings.Audio.MasterVolume");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Audio_Quality, "MCore.Settings.Audio.Quality");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Audio_MusicVolume, "MCore.Settings.Audio.MusicVolume");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Audio_SFXVolume, "MCore.Settings.Audio.SFXVolume");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Audio_VoiceVolume, "MCore.Settings.Audio.VoiceVolume");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Audio_AmbientVolume, "MCore.Settings.Audio.AmbientVolume");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Audio_UIVolume, "MCore.Settings.Audio.UIVolume");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Audio_MuteAudio, "MCore.Settings.Audio.MuteAudio");

    /** Controls Settings */
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Controls_MouseSensitivity, "MCore.Settings.Controls.MouseSensitivity");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Controls_MouseSmoothing, "MCore.Settings.Controls.MouseSmoothing");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Controls_GamepadSensitivity, "MCore.Settings.Controls.GamepadSensitivity");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Controls_SensitivityX, "MCore.Settings.Controls.SensitivityX");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Controls_SensitivityY, "MCore.Settings.Controls.SensitivityY");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Controls_InvertLookX, "MCore.Settings.Controls.InvertLookX");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Controls_InvertLookY, "MCore.Settings.Controls.InvertLookY");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Controls_Vibration, "MCore.Settings.Controls.Vibration");

    /** Accessibility Settings */
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Accessibility_Subtitles, "MCore.Settings.Accessibility.Subtitles");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Accessibility_SubtitleSize, "MCore.Settings.Accessibility.SubtitleSize");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Accessibility_UIScale, "MCore.Settings.Accessibility.UIScale");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Accessibility_ColorblindMode, "MCore.Settings.Accessibility.ColorblindMode");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Accessibility_ColorblindStrength, "MCore.Settings.Accessibility.ColorblindStrength");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Accessibility_TooltipDelay, "MCore.Settings.Accessibility.TooltipDelay");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Accessibility_ScreenReader, "MCore.Settings.Accessibility.ScreenReader");
    UE_DEFINE_GAMEPLAY_TAG(MCore_Settings_Accessibility_LargeText, "MCore.Settings.Accessibility.LargeText");

    /** Utility Functions */
    FGameplayTag GetCategoryTag(const FString& CategoryName)
    {
        if (CategoryName.Equals(TEXT("Display"), ESearchCase::IgnoreCase))
            return MCore_Settings_Category_Display;
        if (CategoryName.Equals(TEXT("Graphics"), ESearchCase::IgnoreCase))
            return MCore_Settings_Category_Graphics;
        if (CategoryName.Equals(TEXT("Audio"), ESearchCase::IgnoreCase))
            return MCore_Settings_Category_Audio;
        if (CategoryName.Equals(TEXT("Controls"), ESearchCase::IgnoreCase))
            return MCore_Settings_Category_Controls;
        if (CategoryName.Equals(TEXT("Accessibility"), ESearchCase::IgnoreCase))
            return MCore_Settings_Category_Accessibility;
        
        return FGameplayTag::EmptyTag;
    }

    FGameplayTag GenerateCustomUISettingTag(const FString& Category, const FString& UISetting)
    {
        FString TagString = FString::Printf(TEXT("%s.%s"), *Category, *UISetting);
        return FGameplayTag::RequestGameplayTag(FName(*TagString));
    }
}
