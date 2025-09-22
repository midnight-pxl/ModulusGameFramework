// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MCore_UITypes.generated.h"

/**
 * Core settings tabs - type-safe alternative to FString
 */
UENUM(BlueprintType, meta=(DisplayName="Settings Tab"))
enum class EMCore_SettingsTab : uint8
{
	None          UMETA(DisplayName="None"),
	Graphics      UMETA(DisplayName="Graphics"),
	Audio         UMETA(DisplayName="Audio"), 
	Controls      UMETA(DisplayName="Controls"),
	Gameplay	  UMETA(DisplayName="Gameplay"),
	Accessibility UMETA(DisplayName="Accessibility"),
	Advanced      UMETA(DisplayName="Advanced")
};

// Simple inline conversion functions (no utility class needed)
inline FString EnumToString(EMCore_SettingsTab Tab)
{
	return StaticEnum<EMCore_SettingsTab>()->GetNameStringByValue(static_cast<uint8>(Tab));
}