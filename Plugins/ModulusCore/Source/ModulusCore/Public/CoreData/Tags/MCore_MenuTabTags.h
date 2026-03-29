// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_MenuTabTags.h
 *
 * Native gameplay tag declarations for the in-game GameMenuHub system.
 * Allows for categorizing and sorting of in-menu tabs by GameplayTag
 */

#pragma once

#include "NativeGameplayTags.h"

namespace MCore_MenuTabTags
{
	// ========================================================================
	// IN GAME MENU TAB TAGS
	// ========================================================================

    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_UI_MenuTab);
    MODULUSCORE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MCore_UI_MenuTab_Settings);
}
