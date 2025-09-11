// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "MCore_EventScope.generated.h"

UENUM()
enum class EMCore_EventScope : uint8
{
	Local	UMETA(DisplayName = "Local Event", ToolTip = "Event that should local to Client (UI, client side only logic)"),
	Global	UMETA(DisplayName = "Global Event", ToolTip = "Event that should broadcast and be replicated globally (to all clients with authority")
};
