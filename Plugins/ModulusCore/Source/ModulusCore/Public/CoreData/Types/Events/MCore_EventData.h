// Copyright 2025, Midnight Pixel Studio LLC.

/**
 * MCore_EventData.h
 *
 * Event data structures for the Modulus tag-based event system,
 * including scope enumeration, parameter storage, and RPC-safe payload.
 */

#pragma once

#include "GameplayTagContainer.h"
#include "MCore_EventData.generated.h"

/**
 * Determines how an event is routed through the system.
 *
 * Blueprint Usage: Select scope when calling BroadcastSimpleEvent or similar.
 * Authority: Local requires no authority; Global requires server validation.
 */
UENUM(BlueprintType)
enum class EMCore_EventScope : uint8
{
	Local	UMETA(DisplayName="Local Only", ToolTip = "This client only - UI updates, audio cues, client-side effects, etc."),
	Global	UMETA(DisplayName="Global (All Players)", ToolTip = "All players - Player actions, gameplay state, multiplayer events, etc.")
};

/* Single key-value parameter entry, RPC-safe */
USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_EventParameter
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Key;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Value;

	FMCore_EventParameter() = default;

	FMCore_EventParameter(const FString& InKey, const FString& InValue)
		: Key(InKey), Value(InValue){}
};

/**
 * RPC-compatible event payload with GameplayTag identifier and optional parameters.
 *
 * Key Features:
 * - Tag-based identification for decoupled listener matching
 * - Optional ContextID for single-identifier events
 * - TArray-based parameters optimized for <10 entries (95% of use cases)
 */
USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_EventData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EventData")
	FGameplayTag EventTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EventData")
	FString ContextID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EventData")
	TArray<FMCore_EventParameter> EventParams;

	FMCore_EventData() = default;

	FMCore_EventData(const FGameplayTag& InEventTag, const FString& InContextID)
		: EventTag(InEventTag), ContextID(InContextID) {}
	
	FMCore_EventData(const FGameplayTag& InEventTag, const TMap<FString, FString>& InEventParams = {})
		: EventTag(InEventTag)
	{
		EventParams.Reserve(InEventParams.Num());
		for (const auto& Pair : InEventParams)
		{
			// Skip invalid entries
			if (!Pair.Key.IsEmpty())
			{
				EventParams.Emplace(Pair.Key, Pair.Value);
			}
		}
	}

	bool IsValid() const { return EventTag.IsValid(); }

	/* Parameter lookup by key, returns DefaultValue if not found */
	FString GetParameter(const FString& Key, const FString& DefaultValue = TEXT("")) const
	{
		// Linear search is optimal for typical 1-8 parameters
		for (const FMCore_EventParameter& Param : EventParams)
		{
			if (Param.Key == Key)
			{
				return Param.Value;
			}
		}
		return DefaultValue;
	}
};