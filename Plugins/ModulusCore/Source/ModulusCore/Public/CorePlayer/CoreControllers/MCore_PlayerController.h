// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "MCore_PlayerController.generated.h"

/**
 * Base player controller for ModulusCore
 *
 * Minimal base class for player controllers. Extend this for your game's player controller.
 * Provides integration points for ModulusCore systems (CommonUI, Events, Settings).
 *
 * Derived Classes:
 * - Your game's player controller
 * - Plugin-specific controllers (if needed)
 *
 * Network: Replicated
 * - One instance per player on server
 * - Owning client has autonomous proxy
 * - Other clients see simulated proxy
 */
UCLASS()
class MODULUSCORE_API AMCore_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMCore_PlayerController();
	
protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	//~ Begin APlayerController Interface
	virtual void SetupInputComponent() override;
	//~ End APlayerController Interface
	
	virtual void OnPossess(APawn* aPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Modulus|Input")
	TArray<TObjectPtr<UInputMappingContext>> DefaultInputContexts;
};
