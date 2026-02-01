// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

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
 * Base PlayerController for ModulusCore
 *
 * Handles:
 * - HUD widget creation and push to GameLayer
 * - Deferred UI setup when PrimaryGameLayout not immediately ready
 * - Input context management (DefaultModulusContext)
 *
 * Usage:
 * 1. Create Blueprint subclass
 * 2. Set HUDWidgetClass to your HUD widget
 * 3. Set as default PlayerController in GameMode
 */
UCLASS()
class MODULUSCORE_API AMCore_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMCore_PlayerController();
	
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

protected:
	//~ Begin APlayerController Interface
	virtual void SetupInputComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnPossess(APawn* aPawn) override;
	//~ End APlayerController Interface

	/**
	 * Widget to push when UI system ready
	 * HUD for gameplay level(s), MainMenu for MainMenu level, etc.) 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCommonActivatableWidget> PrimaryWidgetClass;
	
	/**
	 * Layer to push PrimaryWidgetClass onto.
	 * Default: UI_Layer_Game
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (Categories = "MCore.UI.Layer"))
	FGameplayTag PrimaryWidgetLayer;

	UPROPERTY(EditDefaultsOnly, Category = "Modulus|Input")
	TArray<TObjectPtr<UInputMappingContext>> InputContexts;
	
	/**
 * Called when UISubsystem and PrimaryGameLayout are ready.
 * Default pushes PrimaryWidgetClass to PrimaryWidgetLayer.
 * Override for custom setup (clear layers, push additional widgets, etc.)
 */
	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void OnUISystemReady(UMCore_UISubsystem* UISubsystem);
	virtual void OnUISystemReady_Implementation(UMCore_UISubsystem* UISubsystem);

	/** Widget this controller created. Cleaned up in EndPlay. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UCommonActivatableWidget> PrimaryWidget;
	
private:
	UFUNCTION()
	void OnPrimaryGameLayoutReady(UMCore_PrimaryGameLayout* Layout);
	
	void InitializeUISystem();
	
	bool bUISystemInitialized{false};
};
