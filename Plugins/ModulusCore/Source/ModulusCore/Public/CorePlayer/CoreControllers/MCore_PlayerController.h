// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"

#include "MCore_PlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

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
	AMCore_PlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const;

	UFUNCTION(BlueprintCallable, Category = "Modulus|State")
	void AddPlayerStateTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Modulus|State")
	void RemovePlayerStateTag(FGameplayTag Tag);

	UFUNCTION(BlueprintPure, Category = "Modulus|State")
	bool HasPlayerStateTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintPure, Category = "Modulus|State")
	bool HasAnyPlayerStateTags(const FGameplayTagContainer& BlockTags) const;
	
protected:
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	//~ End AActor Interface

	//~ Begin APlayerController Interface
	virtual void SetupInputComponent() override;
	//~ End APlayerController Interface
	
	virtual void OnPossess(APawn* aPawn) override;

	void ToggleInGameMenu();
	
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "State")
	FGameplayTagContainer PlayerStateTags;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, Category = "Modulus|Input")
	TObjectPtr<UInputMappingContext> DefaultModulusContext;

	UPROPERTY(EditDefaultsOnly, Category = "Modulus|Input")
	TArray<TObjectPtr<UInputMappingContext>> InputContexts;
};
