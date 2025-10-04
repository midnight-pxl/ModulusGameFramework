// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MCore_ActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class EMCore_WidgetInputMode : uint8
{
	Default,		// Use widget's default CommonUI behavior
	GameAndMenu,	// Both game and UI input mode (ECommonInputMode::All)
	Game,			// Game input only, hidden cursor (ECommonInputMode::Game)
	Menu,			// UI input only, visible cursor (ECommonInputMode::Menu)
	Modal			// UI input only, mouse captured (for popups, dialogs, etc.)
};

/**
 * Base class for activatable UI widgets in ModulusCore
 *
 * Extends CommonUI's UCommonActivatableWidget with simplified input mode configuration.
 * Use this as the base class for all menus, HUD elements, and interactive UI.
 *
 * Derived Classes:
 * - Settings menus
 * - Pause menus
 * - HUD widgets
 * - Modal dialogs
 *
 * Blueprint Usage:
 * Set InputMode to control how the widget handles input focus (Game, Menu, GameAndMenu, Modal)
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class MODULUSCORE_API UMCore_ActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UMCore_ActivatableWidget(const FObjectInitializer& ObjectInitializer);

protected:

	/** Input mode for this widget (Default, Game, Menu, GameAndMenu, Modal) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Input")
	EMCore_WidgetInputMode InputMode = EMCore_WidgetInputMode::Default;

public:

	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
};
