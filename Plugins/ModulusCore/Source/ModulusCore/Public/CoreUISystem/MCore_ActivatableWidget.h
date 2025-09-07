// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MCore_ActivatableWidget.generated.h"

class UMCore_UICoordinator;

UENUM(BlueprintType)
enum class EMCore_WidgetInputMode : uint8
{
	Default,		// Use widget's default CommonUI behavior
	GameAndMenu,	// Both game and UI input mode (ECommonInputMode::All)
	Game,			// Game input only, hidden cursor (ECommonInputMode::Game)
	Menu,			// UI input only, visible cursor (ECommonInputMode::Menu)
	Modal			// UI input only, mouse captured (for popups, dialogs, etc.)
};

UENUM(BlueprintType)
enum class EMCore_UIPriority : uint8
{
	Background = 0,		// HUD elements, passive displays
	Normal = 1,			// Standard game menus (inventory, journal, character sheet, etc.)
	Important = 2,		// Dialogue, Quest updates, tutorials
	Critical = 3,		// Trading, important choices
	System = 4,			// Save/Load, settings, error dialogues
};

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class MODULUSCORE_API UMCore_ActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UMCore_ActivatableWidget(const FObjectInitializer& ObjectInitializer);

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Input")
	EMCore_WidgetInputMode InputMode = EMCore_WidgetInputMode::Default;

	/** Priority level for plugin coordination */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Priority")
	EMCore_UIPriority UIPriority = EMCore_UIPriority::Normal;

	/** Plugin identifier for debugging and coordination */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Crosstalk")
	FString PluginIdentifier = TEXT("ModulusCore");

	/** Widget to focus when this widget activates (set in Blueprint/derived classes) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Focus", meta = (BindWidget))
	TObjectPtr<UWidget> WidgetToFocus;

public:

	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	// Coordination getters
	UFUNCTION(BlueprintPure, Category = "UI|Priority")
	EMCore_UIPriority GetUIPriority() const { return UIPriority; }

	UFUNCTION(BlueprintPure, Category = "UI|Identity") 
	FString GetPluginIdentifier() const { return PluginIdentifier; }

	// Focus management helper
	UFUNCTION(BlueprintCallable, Category = "UI|Focus")
	void SetWidgetToFocus(UWidget* Widget);

	// Coordinator access for other plugins
	UFUNCTION(BlueprintCallable, Category = "UI|Refs")
	class UMCore_UICoordinator* GetUICoordinator() const;
	
private:
	void HandleInitialFocus();
};
