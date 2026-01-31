// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreData/Types/Events/MCore_EventData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MCore_EventFunctionLibrary.generated.h"

struct FGameplayTag;
struct FMCore_EventData;
enum class EMCore_EventScope : uint8;

/**
 * Event broadcasting and parameter helpers
 *
 * Stateless functions for cross-system communication via GameplayTag-based events.
 * Routes to Local or Global event subsystems based on scope.
 *
 * Common Uses:
 * - UI updates (player died, quest completed, item acquired)
 * - Cross-plugin communication (ModulusCore → ModulusVault → ModulusChronicles)
 * - Decoupled system notifications (avoid direct dependencies)
 *
 * Blueprint Usage:
 * - BroadcastSimpleEvent() for 90% of cases (signal-only, no data)
 * - BroadcastEventWithContext() when you need 1 identifier (quest ID, player name)
 * - BroadcastEvent() rarely (only for complex multi-parameter cases)
 * 
 * Event Scope:
 * - Local  → This client only (UI updates, local effects, split-screen safe)
 * - Global → Server-authoritative, multicasts to all clients
 */
UCLASS()
class MODULUSCORE_API UMCore_EventFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//~ Start of Broadcast Functions
	
	/**
	 * Broadcast signal-only event (90% of use cases)
	 *
	 * No parameters - listeners query subsystems for current state when notified.
	 * Use for simple notifications where data is already available elsewhere.
	 *
	 * Example: BroadcastSimpleEvent("MCore.Events.Player.LevelUp", Local)
	 * Listeners get notified, then query player controller for new level/stats.
	 *
	 * @param EventTag - Event identifier (e.g., MCore.Events.UI.MenuOpened)
	 * @param EventScope - Local (this client only) or Global (networked to all clients)
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus|Events",
			  meta = (DefaultToSelf = "WorldContext"))
	static void BroadcastSimpleEvent(const UObject* WorldContext,
		FGameplayTag EventTag,
		EMCore_EventScope EventScope = EMCore_EventScope::Local);

	/**
	 * Broadcast event with single context identifier (~9% of use cases)
	 *
	 * Use when you need to include minimal context (quest ID, item name, player name).
	 * Listeners receive the ID and can query relevant systems for details.
	 *
	 * Example: BroadcastEventWithContext("MCore.Events.Quest.Completed", "Quest_MainStory_01", Local)
	 *
	 * @param ContextID - Single identifier (quest ID, item tag, player name, etc.)
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus|Events",
			  meta = (DefaultToSelf = "WorldContext"))
	static void BroadcastEventWithContext(const UObject* WorldContext,
		FGameplayTag EventTag,
		const FString& ContextID,
		EMCore_EventScope EventScope = EMCore_EventScope::Local);

    /**
	 * Broadcast event with multiple parameters (~1% of use cases)
	 *
	 * Only use when you need to pass multiple pieces of data that aren't available elsewhere.
	 * For most cases, use BroadcastSimpleEvent() or BroadcastEventWithContext() instead.
	 *
	 * @param EventParams - Key-value pairs (e.g., {"ItemID": "Sword_001", "Quantity": "5"})
	 */
    UFUNCTION(BlueprintCallable, Category = "Modulus|Events",
              meta = (DefaultToSelf = "WorldContext"))
	static void BroadcastEvent(const UObject* WorldContext,
		FGameplayTag EventTag,
		const TMap<FString, FString>& EventParams,
		EMCore_EventScope EventScope = EMCore_EventScope::Local);
	
	//~ End of Broadcast Functions

	//~ Start of Parameter Accessors
	
	/** Get context ID from event data (for events broadcast with BroadcastEventWithContext) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus|Events")
	static FString GetEventContextID(const FMCore_EventData& EventData);
	
	/** Get raw string parameter (returns DefaultValue if key not found) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus|Events")
	static FString GetEventParameter(const FMCore_EventData& EventData,
		const FString& Key,
		const FString& DefaultValue = TEXT(""));
	
	/** Get parameter as bool (parses "true"/"false"/"1"/"0"/"on"/"off"/"yes"/"no") */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus|Events")
	static bool GetBoolParameter(const FMCore_EventData& EventData,
		const FString& Key,
		bool DefaultValue = false);

	/** Get parameter as int32 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus|Events")
	static int32 GetIntParameter(const FMCore_EventData& EventData,
		const FString& Key,
		int32 DefaultValue = 0);

	/** Get parameter as float (parses string to float) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus|Events")
	static float GetFloatParameter(const FMCore_EventData& EventData,
		const FString& Key,
		float DefaultValue = 0.0f);

private:
	// Internal routing function
	static void RouteEventToSubsystem(const UObject* WorldContext,
		const FMCore_EventData& EventData, EMCore_EventScope EventScope);
};
