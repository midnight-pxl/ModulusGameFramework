// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MCore_MenuTabTypes.generated.h"

class UCommonActivatableWidget;

/**
 * Menu tab registration data for in-game menu hub
 *
 * Lightweight struct storing tab information for plugin screen registration.
 * Each tab displays a button in the menu hub that activates its screen widget.
 *
 * Common Priorities:
 * - 100 = Character/Inventory
 * - 200 = Abilities/Skills
 * - 300 = Quests/Journal  
 * - 400 = Map/World
 * - 500 = Social/Guild
 * - 600 = System/Settings
 */
USTRUCT(BlueprintType)
struct FMCore_MenuTab
{
  	GENERATED_BODY()

public:

  	/** Unique identifier (assigned during registration) */
  	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu Tab")
  	FGameplayTag TabID;

  	/** Widget class to display when tab is active */
  	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu Tab")
  	TSubclassOf<UCommonActivatableWidget> ScreenWidgetClass;

  	/** Sort order (lower = left, higher = right) */
  	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu Tab")
  	int32 Priority{0};

	/** Icon displayed on tab button (optional) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu Tab")
	TObjectPtr<UTexture2D> TabIcon{nullptr};

	/** 
	 * Extract display name from TabID
	 * "MVault.UI.Categories.Inventory" == "Inventory"
	 */
	FText GetDisplayName() const
	{
		if (!TabID.IsValid())
		{
			return FText::FromString("Invalid Tab");
		}
        
		FString TagString = TabID.ToString();
		int32 LastDotIndex;
		if (TagString.FindLastChar('.', LastDotIndex))
		{
			return FText::FromString(TagString.RightChop(LastDotIndex + 1));
		}
        
		return FText::FromString(TagString);
	}

	/** Default constructor */
  	FMCore_MenuTab()
  		: Priority(0)
		, TabIcon(nullptr)
  	{}
};
