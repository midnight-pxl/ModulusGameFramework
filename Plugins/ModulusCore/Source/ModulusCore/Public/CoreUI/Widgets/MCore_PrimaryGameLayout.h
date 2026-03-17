// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_PrimaryGameLayout.h
 *
 * Primary game layout providing 4-layer CommonUI stack architecture.
 * Visual container managed by UISubsystem for layer-based widget display.
 */

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonActivatableWidget.h"
#include "MCore_PrimaryGameLayout.generated.h"

/**
 * Primary game layout providing the 4-layer CommonUI widget stack.
 * Visual container only; access layers via UISubsystem's GetLayerStack and PushWidgetToLayer.
 *
 * Layers: GameLayer (HUD), GameMenuLayer (in-game menus),
 * MenuLayer (full-screen menus), ModalLayer (dialogs).
 */
UCLASS(BlueprintType, Blueprintable, Meta = (DisableNativeTick))
class MODULUSCORE_API UMCore_PrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
    UMCore_PrimaryGameLayout(const FObjectInitializer& ObjectInitializer);

	// ============================================================================
	// LAYER STACKS
	// ============================================================================

	/* HUD and persistent gameplay UI */
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> MCore_GameLayer;

	/* In-game menus (inventory, map, crafting) */
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> MCore_GameMenuLayer;

	/* Full-screen menus (main menu, settings, pause) */
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> MCore_MenuLayer;

	/* Dialogs and confirmation popups */
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> MCore_ModalLayer;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
};
