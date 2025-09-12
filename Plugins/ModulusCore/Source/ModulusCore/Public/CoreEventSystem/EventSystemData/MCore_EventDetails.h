// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "GameplayTagContainer.h"
#include "MCore_EventScope.h"
#include "MCore_EventDetails.generated.h"

enum class EMCore_EventScope : uint8;

USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_EventData
{
    GENERATED_BODY()

public:

    // What happened (gameplay tag)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event", meta = (Categories = "MCore.Event"))
    FGameplayTag EventTag;

    // Broadcast Local to Client or Globally
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
    EMCore_EventScope EventScope = EMCore_EventScope::Local;
    
    // Optional Parameter
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
    FString EventParameter;

    // Constructors
    FMCore_EventData() = default;
    
    FMCore_EventData(const FGameplayTag& InTag, EMCore_EventScope InScope = EMCore_EventScope::Local)
        : EventTag(InTag), EventScope(InScope) {}

    FMCore_EventData(const FGameplayTag& InTag, EMCore_EventScope InScope, const FString& InParameter)
        : EventTag(InTag), EventScope(InScope), EventParameter(InParameter) {}

    bool IsValid() const { return EventTag.IsValid(); }
};