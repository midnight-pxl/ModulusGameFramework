// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "MCore_UISubsystem.generated.h"

class UMCore_PrimaryGameLayout;

/**
 * UI subsystem for accessing the primary game layout
 *
 * Provides access to the PrimaryGameLayout widget and its layer stacks.
 * One instance per LocalPlayer (supports split-screen).
 *
 * Common Uses:
 * - Push menus to MenuLayer (settings, pause menu)
 * - Push HUD to GameLayer (health bar, minimap)
 * - Push modals to ModalLayer (confirmation dialogs)
 *
 * Blueprint Usage:
 * 1. Get UI Subsystem from LocalPlayer
 * 2. GetPrimaryGameLayout() to access layer stacks
 * 3. Use CommonUI AddWidget/RemoveWidget on desired layer
 *
 * Network: Client-only
 * - Zero server overhead (LocalPlayerSubsystem)
 * - Each client manages their own UI independently
 */
UCLASS()
class MODULUSCORE_API UMCore_UISubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:

	/** Get the primary game layout widget for this local player */
	UFUNCTION(BlueprintPure, Category = "UI Layout")
	UMCore_PrimaryGameLayout* GetPrimaryGameLayout() const;

	/**
	 * Register primary game layout (called automatically during layout construction)
	 *
	 * Do not call manually - layout registers itself in NativeOnInitialized().
	 *
	 * @return True if registration succeeded
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Layout")
	bool RegisterPrimaryGameLayout(UMCore_PrimaryGameLayout* InLayout);

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UMCore_PrimaryGameLayout> CachedPrimaryGameLayout;
};
