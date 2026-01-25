// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonActivatableWidget.h"
#include "MCore_PrimaryGameLayout.generated.h"

/**
 * Primary game layout providing 4-layer CommonUI stack architecture.
 * 
 * This is a visual container only - access layers via UISubsystem:
 *   UISubsystem->GetLayerStack(UI_Layers_Game)->AddWidget(WidgetClass);
 *   UISubsystem->PushWidgetToLayer(WidgetClass, TAG_UI_Layer_Menu);
 *   
 * Layers are directly accessible - use standard CommonUI operations:
 *   - GameLayer: HUD and gameplay UI elements
 *   - GameMenuLayer: In-game menus (inventory, map, crafting)
 *   - MenuLayer: Full-screen menus (main menu, settings, pause)
 *   - ModalLayer: Dialogs and confirmation popups
 * 
 * Registers with UISubsystem on Initialization
 */
UCLASS(BlueprintType, Blueprintable, Meta = (DisableNativeTick))
class MODULUSCORE_API UMCore_PrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
    UMCore_PrimaryGameLayout(const FObjectInitializer& ObjectInitializer);
	
    /**
     * Widget Stack References 
     */
	
	/** HUD and persistent gameplay UI */
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> MCore_GameLayer;
	
	/** In-game menus (inventory, map, crafting) */
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> MCore_GameMenuLayer;

	/** Full-screen menus (main menu, settings, pause) */
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> MCore_MenuLayer;

	/** Dialogs and confirmation popups */
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> MCore_ModalLayer;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
};