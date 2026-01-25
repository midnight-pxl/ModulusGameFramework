// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "MCore_PlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * Dedicated Main Menu PlayerController
 */
UCLASS()
class MODULUSCORE_API AMCore_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMCore_PlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

protected:
	//~ Begin APlayerController Interface
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;	
	//~ End APlayerController Interface
	
	/**
	 * Input Actions
	 */
	
	void ToggleInGameMenu();
	
	UPROPERTY(EditDefaultsOnly, Category = "Modulus|Input")
	TObjectPtr<UInputMappingContext> DefaultModulusContext;

	UPROPERTY(EditDefaultsOnly, Category = "Modulus|Input")
	TArray<TObjectPtr<UInputMappingContext>> InputContexts;
};
