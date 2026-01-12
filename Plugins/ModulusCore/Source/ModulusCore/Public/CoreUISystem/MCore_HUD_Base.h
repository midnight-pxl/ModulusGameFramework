// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CoreUISystem/CoreWidgets/MCore_PrimaryGameLayout.h"
#include "MCore_HUD_Base.generated.h"

class UMCore_PrimaryGameLayout;
class UMCore_UISubsystem;

/**
 * Base HUD class for ModulusCore UI Framework.
 * 
 * Usage:
 * Set your GameMode's HUDClass to AMCore_HUD_Base (or derived BP)
 * 
 * @see UMCore_UISubsystem for menu registration and UI service APIs
 * @see UMCore_PrimaryGameLayout for layer stack architecture
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class MODULUSCORE_API AMCore_HUD_Base : public AHUD
{
	GENERATED_BODY()
	
public:
	AMCore_HUD_Base(const FObjectInitializer& ObjectInitializer);
	
	//~ Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor Interface
	
	/**
	 * Returns the PrimaryGameLayout widget owned by this HUD.
	 * May return nullptr if called before BeginPlay or on dedicated server.
	 */
	UFUNCTION(BlueprintPure, Category = "UI|HUD")
	UMCore_PrimaryGameLayout* GetPrimaryGameLayout() const { return PrimaryGameLayout; }
	
	UFUNCTION(BlueprintPure, Category = "UI|HUD")
	bool HasValidPrimaryGameLayout() const { return IsValid(PrimaryGameLayout); }
	
	/**
	 * Cleans up the PrimaryGameLayout widget.
	 * Called from EndPlay.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void CleanupPrimaryGameLayout();

protected:
	/**
	 * Widget class to use for the PrimaryGameLayout.
	 * Override in Blueprint or derived C++ class to use custom layout.
	 * Must be a UMCore_PrimaryGameLayout or derived class.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|HUD")
	TSubclassOf<UMCore_PrimaryGameLayout> PrimaryGameLayoutClass;
	/**
	 * Z-Order for the PrimaryGameLayout when added to viewport.
	 * Default: 0 (behind most other widgets)
	 * Higher values appear on top of lower values.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|HUD", meta = (ClampMin = "-100", ClampMax = "100"))
	int32 PrimaryGameLayoutZOrder = 0;

	/**
	 * Called after PrimaryGameLayout is created and added to viewport.
	 * Override in Blueprint to perform additional setup (e.g., bind events, spawn additional HUD elements).
	 * 
	 * @param Layout The newly created PrimaryGameLayout widget
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Modulus|UI|HUD")
	void OnPrimaryGameLayoutCreated(UMCore_PrimaryGameLayout* Layout);
	virtual void OnPrimaryGameLayoutCreated_Implementation(UMCore_PrimaryGameLayout* Layout);

private:
	/** Cached reference to the PrimaryGameLayout widget */
	UPROPERTY(Transient)
	TObjectPtr<UMCore_PrimaryGameLayout> PrimaryGameLayout;

	/** Cached reference to the UISubsystem for this local player */
	UPROPERTY(Transient)
	TObjectPtr<UMCore_UISubsystem> CachedUISubsystem;

	/**
	 * Creates the PrimaryGameLayout widget and adds it to the viewport.
	 * Called from BeginPlay, skipped on dedicated server.
	 */
	void CreatePrimaryGameLayout();

	/**
	 * Registers the PrimaryGameLayout with the UISubsystem.
	 * Called after successful layout creation.
	 */
	void RegisterLayoutWithSubsystem();

	/**
	 * Returns the UISubsystem for the owning player.
	 * Caches the result for subsequent calls.
	 */
	UMCore_UISubsystem* GetUISubsystem();
};
