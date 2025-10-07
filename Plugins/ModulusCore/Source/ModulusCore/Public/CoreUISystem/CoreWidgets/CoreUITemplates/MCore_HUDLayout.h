// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MCore_HUDLayout.generated.h"

/** HUD NamedSlots for gameplay widgets */
UENUM(BlueprintType)
enum class EMCore_HUDSlot : uint8
{
    /**
     * System-wide notifications spanning top edge
     * Examples: Match timers (Fortnite storm), objective bars (Overwatch payload), server messages
     */
    Top_Margin,
    
    /**
     * Primary player vitals and team status
     * Examples: Health/mana bars (Skyrim), character portrait (LoL), team roster (Valorant)
     */
    Top_Left,
    
    /**
     * Navigational aids and status effects
     * Examples: Compass (Skyrim), buff icons (WoW), round timer (R6 Siege), kill streak (CoD)
     */
    Top_Center,
    
    /**
     * Secondary info and map displays
     * Examples: Minimap (most shooters), currency (LoL gold), FPS counter, enemy stats
     */
    Top_Right,
    
    /**
     * Vertical notifications spanning left edge
     * Examples: Combat log (MMOs), vertical kill feed, achievement tracker
     */
    Left_Margin,
    
    /**
     * Communication and transient notifications
     * Examples: Chat window (multiplayer), item pickups (Destiny 2), loot notifications
     */
    Middle_Left,
    
    /**
     * Core interaction and critical prompts
     * Examples: Crosshair (FPS), interact prompts, boss health bars (Dark Souls), subtitles
     */
    Middle_Center,
    
    /**
     * Active quest tracking and target info
     * Examples: Quest objectives (Skyrim/WoW), target lock-on (Witcher 3), mission steps
     */
    Middle_Right,
    
    /**
     * Vertical status displays spanning right edge
     * Examples: Team roster (Overwatch), squad vitals (BR games), round history (Valorant)
     */
    Right_Margin,
    
    /**
     * Secondary abilities and quick-access items
     * Examples: Ability cooldowns (Overwatch), inventory slots (Fortnite), consumables (MH:W)
     */
    Bottom_Left,
    
    /**
     * Primary action bar and ammunition
     * Examples: Ammo counter (shooters), main hotbar (MMOs), weapon display (Destiny 2)
     */
    Bottom_Center,
    
    /**
     * Resources and status displays
     * Examples: Crafting materials (Fortnite), survival stats (Ark), XP bar (MMOs), buffs
     */
    Bottom_Right
};

/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_HUDLayout : public UCommonUserWidget
{
	GENERATED_BODY()
};
