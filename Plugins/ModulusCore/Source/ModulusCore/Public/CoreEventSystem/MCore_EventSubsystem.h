// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "EventSystemData/MCore_EventScope.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MCore_EventSubsystem.generated.h"

class IMCore_EventListeners;
class FMCore_EventData;

/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_EventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Core broadcasting
	UFUNCTION(BlueprintCallable, Category = "Modulus Events", CallInEditor)
	void BroadcastEvent(const FMCore_EventData& Event);

	// Quick broadcast
	UFUNCTION(BlueprintCallable, Category = "Modulus Events", CallInEditor)
	void BroadcastSimple(const FGameplayTag& EventTag, EMCore_EventScope Scope = EMCore_EventScope::Local,
		const FString& Parameter = TEXT(""));

	// Convenience helpers (optional, but helpful)
	UFUNCTION(BlueprintCallable, Category = "Modulus Events", CallInEditor)
	void BroadcastUIEvent(const FGameplayTag& EventTag, const FString& Parameter = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = "Modulus Events", CallInEditor)
	void BroadcastPlayerEvent(const FGameplayTag& EventTag, const FString& Parameter = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = "Modulus Events", CallInEditor)
	void BroadcastGameplayEvent(const FGameplayTag& EventTag, const FString& Parameter = TEXT(""));

	// Listener management
	void RegisterListener(TScriptInterface<IMCore_EventListeners> Listener);
	void UnregisterListener(TScriptInterface<IMCore_EventListeners> Listener);

	// Static access
	static UMCore_EventSubsystem* Get(const UObject* WorldContext);

protected:
	UPROPERTY()
	TArray<TScriptInterface<IMCore_EventListeners>> EventListeners;

	// Network authority check (uses NetworkingComponent patterns)
	bool HasBroadcastAuthority(const FMCore_EventData& Event) const;

private:
	static TWeakObjectPtr<UMCore_EventSubsystem> CachedSubsystem;









	
};
