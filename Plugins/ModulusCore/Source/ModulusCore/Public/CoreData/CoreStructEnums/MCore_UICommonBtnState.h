// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "MCore_UICommonBtnState.generated.h"

UENUM(BlueprintType)
enum class EMCore_ButtonState : uint8
{
    Normal,
    Hovered,
    Pressed,
    SelectedNormal,
    SelectedHovered,
    Disabled
};