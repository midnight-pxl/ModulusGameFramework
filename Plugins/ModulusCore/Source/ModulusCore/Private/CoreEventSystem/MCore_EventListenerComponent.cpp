// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreEventSystem/MCore_EventListenerComponent.h"

#include "CoreData/CoreLogging/LogModulusEvent.h"
#include "CoreEventSystem/MCore_EventSubsystem.h"


// Sets default values for this component's properties
UMCore_EventListenerComponent::UMCore_EventListenerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bIsListening = true;
	bIsRegistered = false;
}

void UMCore_EventListenerComponent::BeginPlay()
{
	Super::BeginPlay();
	RegisterWithEventSubsystem();
}

void UMCore_EventListenerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterFromEventSubsystem();
	Super::EndPlay(EndPlayReason);
}

void UMCore_EventListenerComponent::SetListening(bool bListening)
{
	if (bIsListening != bListening)
	{
		bIsListening = bListening;
		
		if (bListening)
		{
			RegisterWithEventSubsystem();
		}
		else
		{
			UnregisterFromEventSubsystem();
		}
	}
}

void UMCore_EventListenerComponent::RegisterWithEventSubsystem()
{
	if (!bIsRegistered && bIsListening && IsValid(GetOwner()))
	{
		if (UMCore_EventSubsystem* EventSubsystem = UMCore_EventSubsystem::Get(this))
		{
			TScriptInterface<IMCore_EventListeners> ListenerInterface;
			ListenerInterface.SetObject(this);
			ListenerInterface.SetInterface(Cast<IMCore_EventListeners>(this));

			EventSubsystem->RegisterListener(ListenerInterface);
			bIsRegistered = true;

			UE_LOG(LogModulusEvent, Verbose, TEXT("EventListenerComponent Registered: %s"),
				*GetOwner()->GetName());
		}
	}
}

void UMCore_EventListenerComponent::UnregisterFromEventSubsystem()
{
	if (bIsRegistered)
	{
		if (UMCore_EventSubsystem* EventSubsystem = UMCore_EventSubsystem::Get(this))
		{
			TScriptInterface<IMCore_EventListeners> ListenerInterface;
			ListenerInterface.SetObject(this);
			ListenerInterface.SetInterface(Cast<IMCore_EventListeners>(this));

			EventSubsystem->RegisterListener(ListenerInterface);
			bIsRegistered = false;

			UE_LOG(LogModulusEvent, Verbose, TEXT("EventListenerComponent Unregistered: %s"),
				GetOwner() ? *GetOwner()->GetName() : TEXT("Unknown"));
		}
	}
}
