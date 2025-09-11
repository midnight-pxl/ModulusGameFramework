// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "GameplayTagContainer.h"
#include "MCore_SimpleEvent.generated.h"

USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_SimpleEvent
{
	GENERATED_BODY()

public:

    // What happened (gameplay tag)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event", meta = (Categories = "MCore.Event"))
    FGameplayTag EventTag;

    // Event parameters (flexible key-value storage)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
    TMap<FString, FString> Data;

    // Default constructor
    FMCore_SimpleEvent() = default;

    // Convenience constructor
    explicit FMCore_SimpleEvent(const FGameplayTag& InEventTag)
        : EventTag(InEventTag) {}

    FMCore_SimpleEvent(const FGameplayTag& InEventTag, const TMap<FString, FString>& InData)
        : EventTag(InEventTag), Data(InData) {}

    /**
     * Type-safe parameter helpers (performance optimized)
     */

    // Float parameters
    FORCEINLINE void SetFloat(const FString& Key, float Value)
    {
        Data.Add(Key, FString::SanitizeFloat(Value));
    }

    FORCEINLINE float GetFloat(const FString& Key, float DefaultValue = 0.0f) const
    {
        if (const FString* Value = Data.Find(Key))
        {
            return FCString::Atof(**Value);
        }
        return DefaultValue;
    }

    // Integer parameters
    FORCEINLINE void SetInt(const FString& Key, int32 Value)
    {
        Data.Add(Key, FString::FromInt(Value));
    }

    FORCEINLINE int32 GetInt(const FString& Key, int32 DefaultValue = 0) const
    {
        if (const FString* Value = Data.Find(Key))
        {
            return FCString::Atoi(**Value);
        }
        return DefaultValue;
    }

    // Boolean parameters
    FORCEINLINE void SetBool(const FString& Key, bool Value)
    {
        Data.Add(Key, Value ? TEXT("true") : TEXT("false"));
    }

    FORCEINLINE bool GetBool(const FString& Key, bool DefaultValue = false) const
    {
        if (const FString* Value = Data.Find(Key))
        {
            return Value->ToBool();
        }
        return DefaultValue;
    }

    // Vector parameters
    void SetVector(const FString& Key, const FVector& Value)
    {
        Data.Add(Key, Value.ToString());
    }

    FVector GetVector(const FString& Key, const FVector& DefaultValue = FVector::ZeroVector) const
    {
        if (const FString* Value = Data.Find(Key))
        {
            FVector Result;
            if (Result.InitFromString(*Value))
            {
                return Result;
            }
        }
        return DefaultValue;
    }

    // GameplayTag parameters
    void SetGameplayTag(const FString& Key, const FGameplayTag& Value)
    {
        Data.Add(Key, Value.ToString());
    }

    FGameplayTag GetGameplayTag(const FString& Key, const FGameplayTag& DefaultValue = FGameplayTag()) const
    {
        if (const FString* Value = Data.Find(Key))
        {
            return FGameplayTag::RequestGameplayTag(FName(*Value), false);
        }
        return DefaultValue;
    }

    // String parameters (direct access)
    FORCEINLINE void SetString(const FString& Key, const FString& Value)
    {
        Data.Add(Key, Value);
    }

    FORCEINLINE FString GetString(const FString& Key, const FString& DefaultValue = TEXT("")) const
    {
        if (const FString* Value = Data.Find(Key))
        {
            return *Value;
        }
        return DefaultValue;
    }

    // Validation
    bool IsValid() const
    {
        return EventTag.IsValid();
    }

    // Debug representation
    FString ToString() const
    {
        return FString::Printf(TEXT("Event[%s] Data[%d]"), 
               *EventTag.ToString(), Data.Num());
    }
};