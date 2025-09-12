// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MCore_EventScope.h"
#include "CoreEventSystem/MCore_EventSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MCore_EventBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_EventBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Static broadcasting
	UFUNCTION(BlueprintCallable, Category = "Modulus Events", CallInEditor)
	static void BroadcastEvent(UObject* WorldContext, const FMCore_EventData& Event);

	UFUNCTION(BlueprintCallable, Category = "Modulus Events", CallInEditor)
	static void BroadcastSimple(UObject* WorldContext, const FGameplayTag& EventTag, 
		EMCore_EventScope Scope = EMCore_EventScope::Local, const FString& Parameter = TEXT(""));

	// Convenience helpers with automatic scope
	UFUNCTION(BlueprintCallable, Category = "Modulus Events", CallInEditor)
	static void BroadcastUIEvent(UObject* WorldContext, const FGameplayTag& EventTag, const FString& Parameter = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = "Modulus Events", CallInEditor)
	static void BroadcastPlayerEvent(UObject* WorldContext, const FGameplayTag& EventTag, const FString& Parameter = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = "Modulus Events", CallInEditor)
	static void BroadcastGameplayEvent(UObject* WorldContext, const FGameplayTag& EventTag, const FString& Parameter = TEXT(""));

	UFUNCTION(BlueprintPure, Category = "Modulus Events")
	static UMCore_EventSubsystem* GetEventSubsystem(UObject* WorldContext);
};
