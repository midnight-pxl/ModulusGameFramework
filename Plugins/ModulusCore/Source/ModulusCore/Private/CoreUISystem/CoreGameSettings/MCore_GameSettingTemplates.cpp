// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUISystem/CoreGameSettings/MCore_GameSettingTemplates.h"

/** Display Settings */

FMCore_SettingCategory UMCore_GameSettingTemplates::CreateDisplaySettings()
{
    FMCore_SettingCategory Category;
    Category.CategoryName = FText::FromString("Display");
    Category.CategoryDescription = FText::FromString("Screen Resolution, Window Mode, and Display Options");
    Category.CategoryTag = MCore_UISettingsTags::Settings_Category_Display;

    // Screen Resolution
    FMCore_SettingDefinition Resolution;
    Resolution.DisplayName = FText::FromString("Resolution");
    Resolution.Description = FText::FromString("Screen Resolution");
    Resolution.SettingType = EMCore_SettingType::Dropdown;
    Resolution.SaveKey = MCore_UISettingsTags::Settings_Display_Resolution;
    Resolution.DropdownOptions = {
        FText::FromString("1280x720"),
        FText::FromString("1366x768"),
        FText::FromString("1600x900"),
        FText::FromString("1920x1080"),
        FText::FromString("2560x1080"),
        FText::FromString("2560x1440"),
        FText::FromString("2560x1600"),
        FText::FromString("3440x1440"), 
        FText::FromString("3840x2160"),
    };
    Resolution.DefaultDropdownIndex = 0;
    Category.Settings.Add(Resolution);

    // Window Mode
    FMCore_SettingDefinition WindowMode;
    WindowMode.DisplayName = FText::FromString("Window Mode");
    WindowMode.Description = FText::FromString("Fullscreen, Windowed, or Borderless");
    WindowMode.SettingType = EMCore_SettingType::Dropdown;
    WindowMode.SaveKey = MCore_UISettingsTags::Settings_Display_WindowMode;
    WindowMode.DropdownOptions = {
        FText::FromString("Fullscreen"),
        FText::FromString("Windowed"),
        FText::FromString("Borderless Window")
    };
    WindowMode.DefaultDropdownIndex = 0;
    Category.Settings.Add(WindowMode);

    // VSync
    FMCore_SettingDefinition VSync;
    VSync.DisplayName = FText::FromString("Vertical Sync");
    VSync.Description = FText::FromString("Prevents screen tearing but may reduce performance");
    VSync.SettingType = EMCore_SettingType::Toggle;
    VSync.SaveKey = MCore_UISettingsTags::Settings_Display_VSync;
    VSync.DefaultToggleValue = true;
    Category.Settings.Add(VSync);

    // Frame Rate Limit
    FMCore_SettingDefinition FrameRate;
    FrameRate.DisplayName = FText::FromString("Frame Rate Limit");
    FrameRate.Description = FText::FromString("Maximum frames per second (0 = unlimited)");
    FrameRate.SettingType = EMCore_SettingType::Dropdown;
    FrameRate.SaveKey = MCore_UISettingsTags::Settings_Display_FrameRateLimit;
    FrameRate.DropdownOptions = {
        FText::FromString("Unlimited"),
        FText::FromString("30 FPS"),
        FText::FromString("60 FPS"),
        FText::FromString("120 FPS"),
        FText::FromString("144 FPS"),
        FText::FromString("165 FPS"),
        FText::FromString("220 FPS")
    };
    FrameRate.DefaultDropdownIndex = 2; // 60 FPS
    Category.Settings.Add(FrameRate);

    return Category;
}

/** Graphic Settings */

FMCore_SettingCategory UMCore_GameSettingTemplates::CreateGraphicsSettings()
{
    FMCore_SettingCategory Category;
    Category.CategoryName = FText::FromString("Graphics");
    Category.CategoryDescription = FText::FromString("Visual quality and rendering options");
    Category.CategoryTag = MCore_UISettingsTags::Settings_Category_Graphics;

    // Overall Quality Preset
    FMCore_SettingDefinition QualityPreset;
    QualityPreset.DisplayName = FText::FromString("Quality Preset");
    QualityPreset.Description = FText::FromString("Overall graphics quality level");
    QualityPreset.SettingType = EMCore_SettingType::Dropdown;
    QualityPreset.SaveKey = MCore_UISettingsTags::Settings_Graphics_QualityPreset;
    QualityPreset.DropdownOptions = {
        FText::FromString("Low"),
        FText::FromString("Medium"),
        FText::FromString("High"),
        FText::FromString("Ultra"),
        FText::FromString("Custom")
    };
    QualityPreset.DefaultDropdownIndex = 1; // Medium
    Category.Settings.Add(QualityPreset);

    // Texture Quality
    FMCore_SettingDefinition TextureQuality;
    TextureQuality.DisplayName = FText::FromString("Texture Quality");
    TextureQuality.Description = FText::FromString("Detail level of textures");
    TextureQuality.SettingType = EMCore_SettingType::Dropdown;
    TextureQuality.SaveKey = MCore_UISettingsTags::Settings_Graphics_TextureQuality;
    TextureQuality.DropdownOptions = {
        FText::FromString("Low"),
        FText::FromString("Medium"),
        FText::FromString("High"),
        FText::FromString("Ultra")
    };
    TextureQuality.DefaultDropdownIndex = 1; // Medium
    Category.Settings.Add(TextureQuality);

    // Shadow Quality
    FMCore_SettingDefinition ShadowQuality;
    ShadowQuality.DisplayName = FText::FromString("Shadow Quality");
    ShadowQuality.Description = FText::FromString("Quality and distance of shadows");
    ShadowQuality.SettingType = EMCore_SettingType::Dropdown;
    ShadowQuality.SaveKey = MCore_UISettingsTags::Settings_Graphics_ShadowQuality;
    ShadowQuality.DropdownOptions = {
        FText::FromString("Off"),
        FText::FromString("Low"),
        FText::FromString("Medium"),
        FText::FromString("High")
    };
    ShadowQuality.DefaultDropdownIndex = 2; // Medium
    Category.Settings.Add(ShadowQuality);

    // Anti-Aliasing
    FMCore_SettingDefinition AntiAliasing;
    AntiAliasing.DisplayName = FText::FromString("Anti-Aliasing");
    AntiAliasing.Description = FText::FromString("Smooths jagged edges");
    AntiAliasing.SettingType = EMCore_SettingType::Dropdown;
    AntiAliasing.SaveKey = MCore_UISettingsTags::Settings_Graphics_AntiAliasing;
    AntiAliasing.DropdownOptions = {
        FText::FromString("Off"),
        FText::FromString("FXAA"),
        FText::FromString("TAA"),
        FText::FromString("MSAA 2x"),
        FText::FromString("MSAA 4x")
    };
    AntiAliasing.DefaultDropdownIndex = 2; // TAA
    Category.Settings.Add(AntiAliasing);

    // View Distance
    FMCore_SettingDefinition ViewDistance;
    ViewDistance.DisplayName = FText::FromString("View Distance");
    ViewDistance.Description = FText::FromString("How far objects are rendered");
    ViewDistance.SettingType = EMCore_SettingType::Slider;
    ViewDistance.SaveKey = MCore_UISettingsTags::Settings_Graphics_ViewDistance;
    ViewDistance.MinValue = 0.1f;
    ViewDistance.MaxValue = 1.0f;
    ViewDistance.DefaultValue = 0.7f;
    ViewDistance.StepSize = 0.1f;
    Category.Settings.Add(ViewDistance);

    return Category;
}

/** Audio Settings */

FMCore_SettingCategory UMCore_GameSettingTemplates::CreateAudioSettings()
{
    FMCore_SettingCategory Category;
    Category.CategoryName = FText::FromString("Audio");
    Category.CategoryDescription = FText::FromString("Volume levels and audio options");
    Category.CategoryTag = MCore_UISettingsTags::Settings_Category_Audio;

    // Master Volume
    FMCore_SettingDefinition MasterVolume;
    MasterVolume.DisplayName = FText::FromString("Master Volume");
    MasterVolume.Description = FText::FromString("Overall game volume");
    MasterVolume.SettingType = EMCore_SettingType::Slider;
    MasterVolume.SaveKey = MCore_UISettingsTags::Settings_Audio_MasterVolume;
    MasterVolume.MinValue = 0.0f;
    MasterVolume.MaxValue = 1.0f;
    MasterVolume.DefaultValue = 0.6f;
    MasterVolume.StepSize = 0.01f;
    Category.Settings.Add(MasterVolume);

    // Music Volume
    FMCore_SettingDefinition MusicVolume;
    MusicVolume.DisplayName = FText::FromString("Music Volume");
    MusicVolume.Description = FText::FromString("Background music volume");
    MusicVolume.SettingType = EMCore_SettingType::Slider;
    MusicVolume.SaveKey = MCore_UISettingsTags::Settings_Audio_MusicVolume;
    MusicVolume.MinValue = 0.0f;
    MusicVolume.MaxValue = 1.0f;
    MusicVolume.DefaultValue = 0.7f;
    MusicVolume.StepSize = 0.01f;
    Category.Settings.Add(MusicVolume);

    // SFX Volume
    FMCore_SettingDefinition SFXVolume;
    SFXVolume.DisplayName = FText::FromString("Effects Volume");
    SFXVolume.Description = FText::FromString("Sound effects volume");
    SFXVolume.SettingType = EMCore_SettingType::Slider;
    SFXVolume.SaveKey = MCore_UISettingsTags::Settings_Audio_SFXVolume;
    SFXVolume.MinValue = 0.0f;
    SFXVolume.MaxValue = 1.0f;
    SFXVolume.DefaultValue = 0.8f;
    SFXVolume.StepSize = 0.01f;
    Category.Settings.Add(SFXVolume);

    // Voice Volume
    FMCore_SettingDefinition VoiceVolume;
    VoiceVolume.DisplayName = FText::FromString("Voice Volume");
    VoiceVolume.Description = FText::FromString("Character dialogue volume");
    VoiceVolume.SettingType = EMCore_SettingType::Slider;
    VoiceVolume.SaveKey = MCore_UISettingsTags::Settings_Audio_VoiceVolume;
    VoiceVolume.MinValue = 0.0f;
    VoiceVolume.MaxValue = 1.0f;
    VoiceVolume.DefaultValue = 0.9f;
    VoiceVolume.StepSize = 0.01f;
    Category.Settings.Add(VoiceVolume);

    // Audio Quality
    FMCore_SettingDefinition AudioQuality;
    AudioQuality.DisplayName = FText::FromString("Audio Quality");
    AudioQuality.Description = FText::FromString("Audio processing quality");
    AudioQuality.SettingType = EMCore_SettingType::Dropdown;
    AudioQuality.SaveKey = MCore_UISettingsTags::Settings_Audio_Quality;
    AudioQuality.DropdownOptions = {
        FText::FromString("Low"),
        FText::FromString("Medium"),
        FText::FromString("High")
    };
    AudioQuality.DefaultDropdownIndex = 1; // Medium
    Category.Settings.Add(AudioQuality);

    return Category;
}

//** Controls Settings */

FMCore_SettingCategory UMCore_GameSettingTemplates::CreateControlsSettings()
{
    FMCore_SettingCategory Category;
    Category.CategoryName = FText::FromString("Controls");
    Category.CategoryDescription = FText::FromString("Input sensitivity and key bindings");
    Category.CategoryTag = MCore_UISettingsTags::Settings_Category_Controls;

    // Mouse Sensitivity
    FMCore_SettingDefinition MouseSensitivity;
    MouseSensitivity.DisplayName = FText::FromString("Mouse Sensitivity");
    MouseSensitivity.Description = FText::FromString("How fast the camera moves with mouse input");
    MouseSensitivity.SettingType = EMCore_SettingType::Slider;
    MouseSensitivity.SaveKey = MCore_UISettingsTags::Settings_Controls_MouseSensitivity;
    MouseSensitivity.MinValue = 0.1f;
    MouseSensitivity.MaxValue = 3.0f;
    MouseSensitivity.DefaultValue = 1.0f;
    MouseSensitivity.StepSize = 0.02f;
    Category.Settings.Add(MouseSensitivity);

    // Invert Y Axis
    FMCore_SettingDefinition InvertY;
    InvertY.DisplayName = FText::FromString("Invert Y Axis");
    InvertY.Description = FText::FromString("Invert vertical mouse movement");
    InvertY.SettingType = EMCore_SettingType::Toggle;
    InvertY.SaveKey = MCore_UISettingsTags::Settings_Controls_InvertY;
    InvertY.DefaultToggleValue = false;
    Category.Settings.Add(InvertY);

    // Gamepad Sensitivity
    FMCore_SettingDefinition GamepadSensitivity;
    GamepadSensitivity.DisplayName = FText::FromString("Gamepad Sensitivity");
    GamepadSensitivity.Description = FText::FromString("Controller stick sensitivity");
    GamepadSensitivity.SettingType = EMCore_SettingType::Slider;
    GamepadSensitivity.SaveKey = MCore_UISettingsTags::Settings_Controls_GamepadSensitivity;
    GamepadSensitivity.MinValue = 0.1f;
    GamepadSensitivity.MaxValue = 3.0f;
    GamepadSensitivity.DefaultValue = 1.0f;
    GamepadSensitivity.StepSize = 0.02f;
    Category.Settings.Add(GamepadSensitivity);

    // Key Binding Examples (commonly remapped keys)
    FMCore_SettingDefinition MoveForward;
    MoveForward.DisplayName = FText::FromString("Move Forward");
    MoveForward.Description = FText::FromString("Key to move forward");
    MoveForward.SettingType = EMCore_SettingType::KeyBinding;
    MoveForward.SaveKey = MCore_UISettingsTags::Settings_Controls_MoveForward;
    MoveForward.DefaultKey = EKeys::W;
    Category.Settings.Add(MoveForward);

    FMCore_SettingDefinition Jump;
    Jump.DisplayName = FText::FromString("Jump");
    Jump.Description = FText::FromString("Key to jump");
    Jump.SettingType = EMCore_SettingType::KeyBinding;
    Jump.SaveKey = MCore_UISettingsTags::Settings_Controls_Jump;
    Jump.DefaultKey = EKeys::SpaceBar;
    Category.Settings.Add(Jump);

    return Category;
}

/** Accessibility Settings */

FMCore_SettingCategory UMCore_GameSettingTemplates::CreateAccessibilitySettings()
{
    FMCore_SettingCategory Category;
    Category.CategoryName = FText::FromString("Accessibility");
    Category.CategoryDescription = FText::FromString("Options to improve game accessibility");
    Category.CategoryTag = MCore_UISettingsTags::Settings_Category_Accessibility;

    // Subtitles
    FMCore_SettingDefinition Subtitles;
    Subtitles.DisplayName = FText::FromString("Subtitles");
    Subtitles.Description = FText::FromString("Show text for spoken dialogue");
    Subtitles.SettingType = EMCore_SettingType::Toggle;
    Subtitles.SaveKey = MCore_UISettingsTags::Settings_Accessibility_Subtitles;
    Subtitles.DefaultToggleValue = false;
    Category.Settings.Add(Subtitles);

    // Subtitle Size
    FMCore_SettingDefinition SubtitleSize;
    SubtitleSize.DisplayName = FText::FromString("Subtitle Size");
    SubtitleSize.Description = FText::FromString("Size of subtitle text");
    SubtitleSize.SettingType = EMCore_SettingType::Dropdown;
    SubtitleSize.SaveKey = MCore_UISettingsTags::Settings_Accessibility_SubtitleSize;
    SubtitleSize.DropdownOptions = {
        FText::FromString("Small"),
        FText::FromString("Medium"),
        FText::FromString("Large"),
        FText::FromString("Extra Large")
    };
    SubtitleSize.DefaultDropdownIndex = 1; // Medium
    Category.Settings.Add(SubtitleSize);

    // Colorblind Support
    FMCore_SettingDefinition ColorblindMode;
    ColorblindMode.DisplayName = FText::FromString("Colorblind Support");
    ColorblindMode.Description = FText::FromString("Adjust colors for colorblind players");
    ColorblindMode.SettingType = EMCore_SettingType::Dropdown;
    ColorblindMode.SaveKey = MCore_UISettingsTags::Settings_Accessibility_ColorblindMode;
    ColorblindMode.DropdownOptions = {
        FText::FromString("None"),
        FText::FromString("Protanopia"),
        FText::FromString("Deuteranopia"),
        FText::FromString("Tritanopia")
    };
    ColorblindMode.DefaultDropdownIndex = 0; // None
    Category.Settings.Add(ColorblindMode);

    // UI Scale
    FMCore_SettingDefinition UIScale;
    UIScale.DisplayName = FText::FromString("UI Scale");
    UIScale.Description = FText::FromString("Size of user interface elements");
    UIScale.SettingType = EMCore_SettingType::Slider;
    UIScale.SaveKey = MCore_UISettingsTags::Settings_Accessibility_UIScale;
    UIScale.MinValue = 0.4f;
    UIScale.MaxValue = 2.0f;
    UIScale.DefaultValue = 1.0f;
    UIScale.StepSize = 0.2f;
    Category.Settings.Add(UIScale);

    // Reduce Motion
    FMCore_SettingDefinition ReduceMotion;
    ReduceMotion.DisplayName = FText::FromString("Reduce Motion");
    ReduceMotion.Description = FText::FromString("Reduce screen shake and camera effects");
    ReduceMotion.SettingType = EMCore_SettingType::Toggle;
    ReduceMotion.SaveKey = MCore_UISettingsTags::Settings_Accessibility_ReduceMotion;
    ReduceMotion.DefaultToggleValue = false;
    Category.Settings.Add(ReduceMotion);

    return Category;
}

