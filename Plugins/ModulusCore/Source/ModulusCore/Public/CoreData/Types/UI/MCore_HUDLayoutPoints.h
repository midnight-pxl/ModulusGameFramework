// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MCore_HUDLayoutPoints.generated.h"

/** HUD NamedSlots for gameplay widgets */
UENUM(BlueprintType)
enum class EMCore_HUDSlot : uint8
{
    /**
     * System-wide notifications spanning top edge
     * Examples: Match timers, objective bars, server messages
     */
    Top_Margin,
    
    /**
     * Primary player vitals and team status
     * Examples: Health/mana bars, character portrait, team roster
     */
    Top_Left,
    
    /**
     * Navigational aids and status effects
     * Examples: Compass, buff icons, round timer, kill streak
     */
    Top_Center,
    
    /**
     * Secondary info and map displays
     * Examples: Minimap, currency, FPS counter, enemy stats
     */
    Top_Right,
    
    /**
     * Vertical notifications spanning left edge
     * Examples: Combat log, vertical kill feed, achievement tracker
     */
    Left_Margin,
    
    /**
     * Communication and transient notifications
     * Examples: Chat window, item pickups, loot notifications
     */
    Middle_Left,
    
    /**
     * Core interaction and critical prompts
     * Examples: Crosshair, interact prompts, boss health bars, subtitles
     */
    Middle_Center,
    
    /**
     * Active quest tracking and target info
     * Examples: Quest objectives, locked-on target info, mission steps
     */
    Middle_Right,
    
    /**
     * Vertical status displays spanning right edge
     * Examples: Team roster, squad vitals, round history
     */
    Right_Margin,
    
    /**
     * Secondary abilities and quick-access items
     * Examples: Ability cooldowns, inventory hotbar, consumables
     */
    Bottom_Left,
    
    /**
     * Primary action bar and ammunition
     * Examples: Ammo counter, main hotbar, weapon display
     */
    Bottom_Center,
    
    /**
     * Resources and status displays
     * Examples: Crafting materials, survival stats, XP bar, buffs/debuffs
     */
    Bottom_Right
};
