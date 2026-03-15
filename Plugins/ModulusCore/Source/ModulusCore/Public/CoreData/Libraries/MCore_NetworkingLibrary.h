// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_NetworkingLibrary.h
 *
 * Static network helper functions for UObjects that cannot use UMCore_NetworkingComponent
 * (widgets, subsystems, data assets). Provides authority checks and network mode queries.
 */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MCore_NetworkingLibrary.generated.h"

/* Network mode types for UE multiplayer */
UENUM(BlueprintType)
enum class EMCore_NetMode : uint8
{
	DedicatedServer UMETA(DisplayName = "Dedicated Server"),
	ListenServer	UMETA(DisplayName = "Listen Server (Host)"),
	Client			UMETA(DisplayName = "Client"),
    Standalone		UMETA(DisplayName = "Single-Player")
};

/**
 * Network helpers for UObjects that cannot use UMCore_NetworkingComponent.
 *
 * Provides the same network authority API as UMCore_NetworkingComponent,
 * but as static functions for objects that cannot attach actor components.
 *
 * Key Features:
 * - Network mode queries (server, client, dedicated, listen)
 * - Widget authority helpers (check authority from UI context)
 * - Subsystem authority helpers
 *
 * Blueprint Usage:
 * - Use GetNetworkMode for multi-branch network mode checks
 * - Use IsServer/IsClient for simple boolean checks
 * - Use Widget_* functions from widget Blueprints (DefaultToSelf)
 *
 * Use UMCore_NetworkingComponent instead for Actors and ActorComponents.
 */
UCLASS()
class MODULUSCORE_API UMCore_NetworkingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// ============================================================================
	// NETWORK MODE QUERIES
	// ============================================================================

	/**
	 * Get the current network mode.
	 *
	 * For simple checks, use IsServer/IsClient instead.
	 *
	 * @param WorldContextObject Any object with world context
	 * @return Current network mode
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus|Network",
		meta = (WorldContext = "WorldContextObject", DisplayName = "Get Network Mode"))
	static EMCore_NetMode GetNetworkMode(const UObject* WorldContextObject);

	/**
	 * Check if running on server (dedicated or listen).
	 *
	 * @param WorldContextObject Any object with world context
	 * @return True if dedicated server or listen server
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus|Network",
		meta = (WorldContext = "WorldContextObject", DisplayName = "Is Server"))
	static bool IsServer(const UObject* WorldContextObject);

	/**
	 * Check if running on client (remote or listen).
	 *
	 * @param WorldContextObject Any object with world context
	 * @return True if client or listen server
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus|Network",
		meta = (WorldContext = "WorldContextObject", DisplayName = "Is Client"))
	static bool IsClient(const UObject* WorldContextObject);

	/**
	 * Check if running on dedicated server (no local player).
	 *
	 * @param WorldContextObject Any object with world context
	 * @return True only if dedicated server
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus|Network",
		meta = (WorldContext = "WorldContextObject", DisplayName = "Is Dedicated Server"))
	static bool IsDedicatedServer(const UObject* WorldContextObject);

	/**
	 * Check if running as listen server (hosting with local player).
	 *
	 * @param WorldContextObject Any object with world context
	 * @return True only if listen server
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus|Network",
		meta = (WorldContext = "WorldContextObject", DisplayName = "Is Listen Server"))
	static bool IsListenServer(const UObject* WorldContextObject);

	// ============================================================================
	// WIDGET AUTHORITY HELPERS
	// ============================================================================

	/**
	 * Check if widget's owning player has network authority.
	 * Use before triggering server RPCs from UI.
	 *
	 * @param Widget The widget to check authority for
	 * @return True if player's pawn has authority
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus|Network|Widget",
		meta = (DefaultToSelf = "Widget", DisplayName = "Has Network Authority (Widget)"))
	static bool Widget_HasNetworkAuthority(const UUserWidget* Widget);

	/**
	 * Check if widget can execute server operations.
	 * Validates both authority and valid player controller.
	 *
	 * @param Widget The widget to check
	 * @return True if server operations are allowed
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus|Network|Widget",
		meta = (DefaultToSelf = "Widget", DisplayName = "Can Execute Server Operation (Widget)"))
	static bool Widget_CanExecuteServerOperation(const UUserWidget* Widget);

	/**
	 * Check if widget can execute client operations.
	 * Returns false on dedicated server (no local player).
	 *
	 * @param Widget The widget to check
	 * @return True if client operations are allowed
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus|Network|Widget",
		meta = (DefaultToSelf = "Widget", DisplayName = "Can Execute Client Operation (Widget)"))
	static bool Widget_CanExecuteClientOperation(const UUserWidget* Widget);

	// ============================================================================
	// WIDGET CONTEXT ACCESS
	// ============================================================================

	/**
	 * Get the widget's owning player controller.
	 *
	 * @param Widget The widget to get owner for
	 * @return Owning player controller, or nullptr on dedicated server
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus|Network|Widget",
		meta = (DefaultToSelf = "Widget", DisplayName = "Get Owning Player Controller (Widget)"))
	static APlayerController* Widget_GetOwningPlayerController(const UUserWidget* Widget);

	/**
	 * Get the widget's owning player's pawn.
	 *
	 * @param Widget The widget to get pawn for
	 * @return Owning pawn, or nullptr if not possessed
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus|Network|Widget",
		meta = (DefaultToSelf = "Widget", DisplayName = "Get Owning Pawn (Widget)"))
	static APawn* Widget_GetOwningPawn(const UUserWidget* Widget);

	// ============================================================================
	// SUBSYSTEM HELPERS
	// ============================================================================

	/**
	 * Check if subsystem is running on server.
	 *
	 * @param Subsystem The subsystem to check
	 * @return True if running on server
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus|Network|Subsystem",
		meta = (DefaultToSelf = "Subsystem", DisplayName = "Is Server (Subsystem)"))
	static bool Subsystem_IsServer(const USubsystem* Subsystem);

	/**
	 * Check if subsystem can execute server operations.
	 *
	 * @param Subsystem The subsystem to check
	 * @return True if server operations are allowed
	 */
	UFUNCTION(BlueprintPure, Category = "Modulus|Network|Subsystem",
		meta = (DefaultToSelf = "Subsystem", DisplayName = "Can Execute Server Operation (Subsystem)"))
	static bool Subsystem_CanExecuteServerOperation(const USubsystem* Subsystem);
};
