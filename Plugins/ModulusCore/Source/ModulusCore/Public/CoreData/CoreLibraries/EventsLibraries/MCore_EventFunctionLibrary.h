// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreData/CoreStructEnums/EventStructsEnums/MCore_EventData.h"
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
 */
UCLASS()
class MODULUSCORE_API UMCore_EventFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
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
	UFUNCTION(BlueprintCallable, Category = "Modulus Events",
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
	UFUNCTION(BlueprintCallable, Category = "Modulus Events",
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
    UFUNCTION(BlueprintCallable, Category = "Modulus Events",
              meta = (DefaultToSelf = "WorldContext"))
	static void BroadcastEvent(const UObject* WorldContext,
		FGameplayTag EventTag,
		const TMap<FString, FString>& EventParams,
		EMCore_EventScope EventScope = EMCore_EventScope::Local);

	/**
	 * Broadcast local event with parameters
	 * Convenience function - same as BroadcastEvent() with EventScope = Local
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus Events",
			  meta = (DefaultToSelf = "WorldContext"))
	static void BroadcastLocalEvent(const UObject* WorldContext,
								   FGameplayTag EventTag,
								   const TMap<FString, FString>& Parameters);

	/**
	 * Broadcast global event with parameters
	 * Convenience function - same as BroadcastEvent() with EventScope = Global
	 *
	 * Network: Server-authoritative
	 * - Called from server: Broadcasts to all clients
	 * - Called from client: Warning logged, no broadcast
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus Events",
			  meta = (DefaultToSelf = "WorldContext"))
	static void BroadcastGlobalEvent(const UObject* WorldContext,
									FGameplayTag EventTag,
									const TMap<FString, FString>& Parameters);

	/** Create event data structure from tag and parameters */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
    static FMCore_EventData MakeEventData(FGameplayTag EventTag,
    	const TMap<FString, FString>& Parameters);

	/** Create parameter map from parallel arrays (Blueprint helper) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static TMap<FString, FString> MakeEventParameters(const TArray<FString>& Keys,
		const TArray<FString>& Values);

	/** Get context ID from event data (for events broadcast with BroadcastEventWithContext) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static FString GetEventContextID(const FMCore_EventData& EventData);

	/** Get raw string parameter (returns DefaultValue if key not found) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static FString GetEventParameter(const FMCore_EventData& EventData, const FString& Key, const FString& DefaultValue = TEXT(""));

	/** Get parameter as bool (parses "true"/"false"/"1"/"0") */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static bool GetBoolParameter(const FMCore_EventData& EventData, const FString& Key,
		bool DefaultValue = false);

	/** Get parameter as int32 (parses string to integer) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static int32 GetIntParameter(const FMCore_EventData& EventData, const FString& Key,
		int32 DefaultValue = 0);

	/** Get parameter as float (parses string to float) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static float GetFloatParameter(const FMCore_EventData& EventData, const FString& Key,
		float DefaultValue = 0.0f);

	/** Get parameter as string (alias for GetEventParameter) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static FString GetStringParameter(const FMCore_EventData& EventData, const FString& Key,
		const FString& DefaultValue = TEXT(""));

	/** Get parameter as FVector (parses string like "X=1.0,Y=2.0,Z=3.0") */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static FVector GetVectorParameter(const FMCore_EventData& EventData, const FString& Key,
		const FVector& DefaultValue = FVector::ZeroVector);

	/** Get parameter as FGameplayTag (parses tag string) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static FGameplayTag GetGameplayTagParameter(const FMCore_EventData& EventData,
		const FString& Key, const FGameplayTag& DefaultValue = FGameplayTag());

private:
	// Internal routing function
	static void RouteEventToSubsystem(const UObject* WorldContext, const FMCore_EventData& EventData, EMCore_EventScope EventScope);
};
