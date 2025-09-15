// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MCore_EventData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MCore_EventFunctionLibrary.generated.h"

struct FGameplayTag;
struct FMCore_EventData;
enum class EMCore_EventScope : uint8;

/**
 * For event broadcasting and parameter helpers
 * No components needed - stateless broadcasting functions
 * Routes to appropriate subsystem based on event scope
 */
UCLASS()
class MODULUSCORE_API UMCore_EventFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Main Function: Broadcast signal-only event (90% of use cases)
	 * No parameters - listeners query relevant subsystems for current state
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus Events", 
			  meta = (DefaultToSelf = "WorldContext"))
	static void BroadcastSimpleEvent(const UObject* WorldContext,
									FGameplayTag EventTag,
									EMCore_EventScope EventScope = EMCore_EventScope::Local);

	/**
	 * Common Function: Event with single context ID (~9% of use cases)
	 * For events that need minimal context like quest ID, player name, etc.
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus Events", 
			  meta = (DefaultToSelf = "WorldContext"))
	static void BroadcastEventWithContext(const UObject* WorldContext,
										 FGameplayTag EventTag,
										 const FString& ContextID,
										 EMCore_EventScope EventScope = EMCore_EventScope::Local);
	
    /**
	 * Rare Function: Event with full parameters (~1% of use cases)
	 * Only for complex edge cases requiring multiple parameters
	 */
    UFUNCTION(BlueprintCallable, Category = "Modulus Events", 
              meta = (DefaultToSelf = "WorldContext"))
	static void BroadcastEvent(const UObject* WorldContext,
		FGameplayTag EventTag,
		const TMap<FString, FString>& EventParams,
		EMCore_EventScope EventScope = EMCore_EventScope::Local);
	
	/**
	 * Simple event broadcasting with local scope (most common case)
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus Events", 
			  meta = (DefaultToSelf = "WorldContext"))
	static void BroadcastLocalEvent(const UObject* WorldContext,
								   FGameplayTag EventTag,
								   const TMap<FString, FString>& Parameters);

	/**
	 * Simple event broadcasting with global scope
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus Events", 
			  meta = (DefaultToSelf = "WorldContext"))
	static void BroadcastGlobalEvent(const UObject* WorldContext,
									FGameplayTag EventTag,
									const TMap<FString, FString>& Parameters);

    /**
     * Helper to create complete event data
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
    static FMCore_EventData MakeEventData(FGameplayTag EventTag,
    	const TMap<FString, FString>& Parameters);

	/**
	 * Helper to create event parameters map
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static TMap<FString, FString> MakeEventParameters(const TArray<FString>& Keys,
		const TArray<FString>& Values);
	
	/**
	 * Parameter helpers for the rare complex events
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static FString GetEventContextID(const FMCore_EventData& EventData);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static FString GetEventParameter(const FMCore_EventData& EventData, const FString& Key, const FString& DefaultValue = TEXT(""));
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static bool GetBoolParameter(const FMCore_EventData& EventData, const FString& Key,
		bool DefaultValue = false);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static int32 GetIntParameter(const FMCore_EventData& EventData, const FString& Key,
		int32 DefaultValue = 0);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static float GetFloatParameter(const FMCore_EventData& EventData, const FString& Key,
		float DefaultValue = 0.0f);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static FString GetStringParameter(const FMCore_EventData& EventData, const FString& Key,
		const FString& DefaultValue = TEXT(""));

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static FVector GetVectorParameter(const FMCore_EventData& EventData, const FString& Key,
		const FVector& DefaultValue = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	static FGameplayTag GetGameplayTagParameter(const FMCore_EventData& EventData,
		const FString& Key, const FGameplayTag& DefaultValue = FGameplayTag());

private:
	// Internal routing function
	static void RouteEventToSubsystem(const UObject* WorldContext, const FMCore_EventData& EventData, EMCore_EventScope EventScope);
};
