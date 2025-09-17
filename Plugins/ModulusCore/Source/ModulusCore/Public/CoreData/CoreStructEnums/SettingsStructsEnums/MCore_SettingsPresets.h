// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MCore_SettingsPresets.generated.h"

// Graphics preset options
UENUM(BlueprintType)
enum class EMCore_GraphicsPreset
{
	Low         UMETA(DisplayName = "Low"),
	Medium      UMETA(DisplayName = "Medium"), 
	High        UMETA(DisplayName = "High"),
	Ultra       UMETA(DisplayName = "Ultra"),
	Custom      UMETA(DisplayName = "Custom")
};

// Audio quality levels
UENUM(BlueprintType)
enum class EMCore_AudioQuality : uint8
{
    Low_22kHz       UMETA(DisplayName = "Low (22kHz)"),
    Medium_44kHz    UMETA(DisplayName = "Medium (44kHz)"),
    High_48kHz      UMETA(DisplayName = "High (48kHz)")
};

// Anti-aliasing types
UENUM(BlueprintType)
enum class EMCore_AntiAliasingType : uint8
{
    None        UMETA(DisplayName = "Off"),
    FXAA        UMETA(DisplayName = "FXAA"),
    TAA         UMETA(DisplayName = "TAA"),
    MSAA_2x     UMETA(DisplayName = "MSAA 2x"),
    MSAA_4x     UMETA(DisplayName = "MSAA 4x"),
    MSAA_8x     UMETA(DisplayName = "MSAA 8x")
};

// Spatial audio modes
UENUM(BlueprintType)
enum class EMCore_SpatialAudioMode : uint8
{
    Off          UMETA(DisplayName = "Off"),
    Stereo       UMETA(DisplayName = "Stereo"),
    Surround_5_1 UMETA(DisplayName = "5.1 Surround"),
    Surround_7_1 UMETA(DisplayName = "7.1 Surround"),
    Atmos        UMETA(DisplayName = "Dolby Atmos")
};

// Input device types
UENUM(BlueprintType)
enum class EMCore_InputDeviceType : uint8
{
    KeyboardMouse   UMETA(DisplayName = "Keyboard & Mouse"),
    Gamepad         UMETA(DisplayName = "Gamepad"),
    Touch           UMETA(DisplayName = "Touch"),
    Unknown         UMETA(DisplayName = "Unknown")
};

// Platform types for optimization
UENUM(BlueprintType)
enum class EMCore_PlatformType : uint8
{
    PC          UMETA(DisplayName = "PC"),
    Console     UMETA(DisplayName = "Console"),
    Mobile      UMETA(DisplayName = "Mobile"),
    SteamDeck   UMETA(DisplayName = "Steam Deck"),
    Unknown     UMETA(DisplayName = "Unknown")
};

// Colorblind support types
UENUM(BlueprintType)
enum class EMCore_ColorBlindType : uint8
{
    None            UMETA(DisplayName = "None"),
    Protanopia      UMETA(DisplayName = "Protanopia"),
    Deuteranopia    UMETA(DisplayName = "Deuteranopia"), 
    Tritanopia      UMETA(DisplayName = "Tritanopia")
};

// Upscaling methods
UENUM(BlueprintType)
enum class EMCore_UpscalingMethod : uint8
{
    None        UMETA(DisplayName = "Off"),
    DLSS        UMETA(DisplayName = "NVIDIA DLSS"),
    FSR         UMETA(DisplayName = "AMD FSR"),
    XeSS        UMETA(DisplayName = "Intel XeSS"),
    TSR         UMETA(DisplayName = "Temporal Super Resolution")
};