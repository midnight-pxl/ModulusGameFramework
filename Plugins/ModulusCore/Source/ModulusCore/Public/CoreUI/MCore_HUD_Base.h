// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CoreUI/Widgets/MCore_PrimaryGameLayout.h"
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
	
	/**
	 * Returns the PrimaryGameLayout widget owned by this HUD.
	 * May return nullptr if called before BeginPlay or on dedicated server.
	 */
	UFUNCTION(BlueprintPure, Category = "UI|HUD")
	UMCore_PrimaryGameLayout* GetPrimaryGameLayout() const;
	
	UFUNCTION(BlueprintPure, Category = "UI|HUD")
	bool HasValidPrimaryGameLayout() const;
	
	/**
	 * Returns the UISubsystem for the owning player.
	 * Caches the result for subsequent calls.
	 */
	UFUNCTION(BlueprintPure, Category = "UI|HUD")
	UMCore_UISubsystem* GetUISubsystem() const;

protected:
	//~ Begin AHUD Interface
	/** Override for canvas-based drawing (debug viz, crosshairs, etc.) */
	virtual void DrawHUD() override;
	//~ End AHUD Interface
};
