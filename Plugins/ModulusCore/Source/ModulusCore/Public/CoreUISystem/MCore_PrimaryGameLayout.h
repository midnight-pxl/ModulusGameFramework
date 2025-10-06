// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonActivatableWidget.h"
#include "MCore_PrimaryGameLayout.generated.h"

/**
 * Primary game layout providing 4-layer CommonUI stack architecture.
 * 
 * Layers are directly accessible - use standard CommonUI operations:
 *   - GameLayer: HUD and gameplay UI elements
 *   - GameMenuLayer: In-game menus (inventory, crafting)
 *   - MenuLayer: Main menus and settings screens
 *   - ModalLayer: Dialogs and confirmation popups
 * 
 * Example Usage:
 *   PrimaryLayout->MenuLayer->AddWidget(SettingsWidgetClass);
 *   PrimaryLayout->ModalLayer->AddWidget(ConfirmDialogClass);
 */
UCLASS(BlueprintType, Blueprintable, Meta = (DisableNativeTick))
class MODULUSCORE_API UMCore_PrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
    UMCore_PrimaryGameLayout(const FObjectInitializer& ObjectInitializer);

    // Widget Stack References
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> MCore_GameLayer;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> MCore_GameMenuLayer;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> MCore_MenuLayer;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> MCore_ModalLayer;

private:
	virtual void NativeOnInitialized() override;
};