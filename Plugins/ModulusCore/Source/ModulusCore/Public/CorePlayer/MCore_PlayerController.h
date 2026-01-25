// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "MCore_PlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class UMCore_PrimaryGameLayout;
class UMCore_ActivatableBase;

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
	
	/**
	 * UI Layer Management
	 */
	
	/** Push a widget onto the specified layer */
	UFUNCTION(BlueprintCallable, Category = "Modulus|UI")
	UMCore_ActivatableBase* PushWidgetToLayer(TSubclassOf<UMCore_ActivatableBase> WidgetClass, FGameplayTag LayerTag);

	/** Remove a widget from its layer */
	UFUNCTION(BlueprintCallable, Category = "Modulus|UI")
	void RemoveWidget(UMCore_ActivatableBase* Widget);

	/** Get the primary game layout */
	UFUNCTION(BlueprintPure, Category = "Modulus|UI")
	UMCore_PrimaryGameLayout* GetPrimaryGameLayout() const { return PrimaryGameLayout; }
	
	/** Check if UI layers are ready */
	UFUNCTION(BlueprintCallable, Category = "Modulus|UI")
	bool IsLayoutReady() const { return PrimaryGameLayout != nullptr; }

protected:
	//~ Begin APlayerController Interface
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;	
	//~ End APlayerController Interface
	
	/**
	 * UI Setup (Override points)
	 */
	
	/** Called after PrimaryGameLayout is ready. Override to push initial widgets */
	UFUNCTION(BlueprintNativeEvent, Category = "Modulus|UI")
	void OnPGLayoutReady();
	
	/** Returns false to prevent layout creation (persisted Controller, Dedicated Server, Spectator, etc.) */
	UFUNCTION(BlueprintNativeEvent, Category = "Modulus|UI")
	bool ShouldCreatePGLayout() const;
	
	/**
	 * Input Actions
	 */
	
	void ToggleInGameMenu();
	
	UPROPERTY(EditDefaultsOnly, Category = "Modulus|Input")
	TObjectPtr<UInputMappingContext> DefaultModulusContext;

	UPROPERTY(EditDefaultsOnly, Category = "Modulus|Input")
	TArray<TObjectPtr<UInputMappingContext>> InputContexts;
	
	/**
	 * UI Configuration
	 */

	/** The primary game layout class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Modulus|UI")
	TSubclassOf<UMCore_PrimaryGameLayout> PrimaryGameLayoutClass;

	
private:
	void CreatePGLayout();
	void SetMenuInputMode();

	UPROPERTY(Transient)
	TObjectPtr<UMCore_PrimaryGameLayout> PrimaryGameLayout;
	
	
};
