// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_PlayerController.h
 *
 * Base PlayerController handling HUD widget creation, deferred UI setup
 * when PrimaryGameLayout is not immediately ready, and input context management.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "GameFramework/PlayerController.h"
#include "MCore_PlayerController.generated.h"

class UInputAction;
class UMCore_UISubsystem;
class UInputMappingContext;
class UCommonActivatableWidget;

/**
 * Base PlayerController for ModulusCore.
 *
 * Key Features:
 * - HUD widget creation and push to GameLayer
 * - Deferred UI setup when PrimaryGameLayout not immediately ready
 * - Input context management (DefaultModulusContext)
 *
 * Blueprint Usage:
 * 1. Create Blueprint subclass
 * 2. Set PrimaryWidgetClass to your HUD widget
 * 3. Set as default PlayerController in GameMode
 */
UCLASS()
class MODULUSCORE_API AMCore_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMCore_PlayerController();

	// ============================================================================
	// ACTOR OVERRIDES
	// ============================================================================

	virtual void BeginPlay() override;

protected:
	// ============================================================================
	// PLAYERCONTROLLER OVERRIDES
	// ============================================================================

	virtual void SetupInputComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnPossess(APawn* aPawn) override;

	/* Widget to push when UI system ready (HUD, MainMenu, etc.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCommonActivatableWidget> PrimaryWidgetClass;

	/* Layer to push PrimaryWidgetClass onto. Default: MCore_UI_Layer_Game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (Categories = "MCore.UI.Layer"))
	FGameplayTag PrimaryWidgetLayer;

	UPROPERTY(EditDefaultsOnly, Category = "Modulus|Input")
	TArray<TObjectPtr<UInputMappingContext>> InputContexts;

	/**
	 * Called when UISubsystem and PrimaryGameLayout are ready.
	 * Default pushes PrimaryWidgetClass to PrimaryWidgetLayer.
	 * Override for custom setup (clear layers, push additional widgets, etc.)
	 *
	 * @param UISubsystem The local player's UI subsystem
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void OnUISystemReady(UMCore_UISubsystem* UISubsystem);
	virtual void OnUISystemReady_Implementation(UMCore_UISubsystem* UISubsystem);

	UPROPERTY(Transient, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UCommonActivatableWidget> PrimaryWidget;

private:
	UFUNCTION()
	void OnPrimaryGameLayoutReady(UMCore_PrimaryGameLayout* Layout);

	void InitializeUISystem();

	bool bUISystemInitialized{false};
};
