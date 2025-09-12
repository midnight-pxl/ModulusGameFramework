// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreEventSystem/EventSystemData/MCore_EventDetails.h"
#include "CoreNetworkingSystem/MCore_NetworkingComponent.h"
#include "MCore_EventListenerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModulusEvent, const FMCore_EventData&, Event);

/**
 * Simple event listener interface
 * Automatically discover and register ActorComponent
 */
UINTERFACE()
class UMCore_EventListeners : public UInterface
{
	GENERATED_BODY()
};

class IMCore_EventListeners
{
	GENERATED_BODY()

public:
	// Handle received events. Implement this function to respond
	UFUNCTION(BlueprintImplementableEvent, Category="ModulusEvents")
	void HandleEvent(const FMCore_EventData& Event);
};

/**
 * Component for actors that want to listen to events.
 * Automatically registers/unregisters with the MCore_EventSubsystem
 */
UCLASS(ClassGroup=(ModulusCore), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class MODULUSCORE_API UMCore_EventListenerComponent : public UMCore_NetworkingComponent, public IMCore_EventListeners
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMCore_EventListenerComponent();

	// Whether this listener is currently active
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Listener")
	bool bIsListening{true};

	// Blueprint event for handling events
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModulusEvent, const FMCore_EventData&, Event);
	UPROPERTY(BlueprintAssignable, Category = "Modulus Events")
	FOnModulusEvent OnModulusEvent;

	//~ Start of UActorComponent Interface

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//~ Start of IMCore_EventListener Interface

	// Blueprint implementation - forwards to delegate
	UFUNCTION(BlueprintImplementableEvent, Category = "Modulus Events")
	void HandleEvent(const FMCore_EventData& Event);

	//~ Start of Blueprint Interface

	/**
	 * Enable or disable event listening at runtime.
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus Events")
	void SetListening(bool bListening);
	
	/**
	 * Check if this listener is currently active.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Modulus Events")
	bool IsListening() const { return bIsListening && IsValid(GetOwner()); }

private:
	// Whether we're currently registered with the event subsystem
	bool bIsRegistered{false};

	// Register/unregister with event subsystem
	void RegisterWithEventSubsystem();
	void UnregisterFromEventSubsystem();
};
