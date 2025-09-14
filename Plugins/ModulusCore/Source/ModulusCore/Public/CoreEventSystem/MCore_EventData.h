// Copyright 2025, Midnight Pixel Studio LLC.

#pragma once

#include "GameplayTagContainer.h"
#include "MCore_EventData.generated.h"

/**
 * Authority validation strictness levels
 */
UENUM(BlueprintType)
enum class EMCore_ValidationStrictness : uint8
{
	Permissive  UMETA(DisplayName = "Permissive", ToolTip = "Graceful degradation, warnings for authority issues"),
	Balanced    UMETA(DisplayName = "Balanced", ToolTip = "Standard validation with clear error reporting"),
	Strict      UMETA(DisplayName = "Strict", ToolTip = "Fail-fast validation, good for competitive games")
};

/**
 * Event scope enumeration - determines routing behavior
 */
UENUM(BlueprintType)
enum class EMCore_EventScope : uint8
{
	Local	UMETA(DisplayName="Local Only", ToolTip = "This client only - UI updates, audio cues, client-side effects, etc."),
	Global	UMETA(DisplayName="Global (All Players)", ToolTip = "All players - Player actions, gameplay state, multiplayer events, etc.")
};

/**
 * Single parameter entry - keeps key and value together (RPC-safe)
 */
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

	bool IsValid() const { return !Key.IsEmpty(); }
};

/**
 * Event data structure - RPC-compatible with fast lookup helpers
 * Optimized for 1-8 parameters (95% of use cases)
 */
USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_EventData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EventData")
	FGameplayTag EventTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EventData")
	TArray<FMCore_EventParameter> EventParams;

	FMCore_EventData() = default;

	FMCore_EventData(const FGameplayTag& InEventTag, const TMap<FString, FString>& InEventParams = {})
		: EventTag(InEventTag)
	{
		EventParams.Reserve(InEventParams.Num());
		for (const auto& Pair : InEventParams)
		{
			if (!Pair.Key.IsEmpty()) // Skip invalid entries
			{
				EventParams.Emplace(Pair.Key, Pair.Value);
			}
		}
	}

	// Quick single-parameter constructor
	FMCore_EventData(const FGameplayTag& InEventTag, const FString& Key, const FString& Value)
		: EventTag(InEventTag)
	{
		EventParams.Emplace(Key, Value);
	}

	bool IsValid() const { return EventTag.IsValid(); }

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

	void AddParameter(const FString& Key, const FString& Value)
	{
		if (!Key.IsEmpty())
		{
			for (FMCore_EventParameter& Param : EventParams)
			{
				if (Param.Key == Key)
				{
					Param.Value = Value;
					return;
				}
			}
			EventParams.Emplace(Key, Value);
		}
	}

	TMap<FString, FString> GetParametersAsMap() const
	{
		TMap<FString, FString> Result;
		Result.Reserve(EventParams.Num());
		for (const FMCore_EventParameter& Param : EventParams)
		{
			if (!Param.Key.IsEmpty())
			{
				Result.Add(Param.Key, Param.Value);
			}
		}
		return Result;
	}
};