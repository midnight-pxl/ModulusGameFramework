// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreData/Assets/Settings/MCore_DA_GameSettings_Organization.h"

UMCore_DA_GameSettings_Organization::UMCore_DA_GameSettings_Organization()
{
    /** Initialize UI categories for organization */
    GraphicsCategories = {
        NSLOCTEXT("ModulusSettings", "Display", "Display & Resolution"),
        NSLOCTEXT("ModulusSettings", "Quality", "Graphics Quality"),
        NSLOCTEXT("ModulusSettings", "Advanced", "Advanced Graphics"),
        NSLOCTEXT("ModulusSettings", "PostProcess", "Post Processing")
    };
    
    AudioCategories = {
        NSLOCTEXT("ModulusSettings", "Volume", "Volume Controls"),
        NSLOCTEXT("ModulusSettings", "Devices", "Audio Devices"),
        NSLOCTEXT("ModulusSettings", "Spatial", "Spatial Audio"),
        NSLOCTEXT("ModulusSettings", "Voice", "Voice & Communication")
    };
    
    ControlsCategories = {
        NSLOCTEXT("ModulusSettings", "Input", "Input Devices"),
        NSLOCTEXT("ModulusSettings", "KeyBindings", "Key Bindings"),
        NSLOCTEXT("ModulusSettings", "Sensitivity", "Sensitivity"),
        NSLOCTEXT("ModulusSettings", "Gamepad", "Gamepad Settings")
    };
    
    AccessibilityCategories = {
        NSLOCTEXT("ModulusSettings", "Visual", "Visual Accessibility"),
        NSLOCTEXT("ModulusSettings", "Audio", "Hearing Accessibility"),
        NSLOCTEXT("ModulusSettings", "Motor", "Motor Accessibility"),
        NSLOCTEXT("ModulusSettings", "Cognitive", "Cognitive Accessibility")
    };

    /** Platform defaults - simple configuration */
    PlatformGraphicsDefaults = {
        {EMCore_PlatformType::PC, EMCore_GraphicsPreset::Ultra},
        {EMCore_PlatformType::Console, EMCore_GraphicsPreset::High},
        {EMCore_PlatformType::Mobile, EMCore_GraphicsPreset::Low},
        {EMCore_PlatformType::SteamDeck, EMCore_GraphicsPreset::Medium}
    };
}
