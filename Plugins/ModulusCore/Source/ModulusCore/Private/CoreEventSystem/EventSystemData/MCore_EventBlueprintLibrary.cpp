// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreEventSystem/EventSystemData/MCore_EventBlueprintLibrary.h"
#include "CoreData/CoreLogging/LogModulusEvent.h"

void UMCore_EventBlueprintLibrary::BroadcastEvent(const UObject* WorldContext, const FMCore_EventData& Event)
{
	if (UMCore_EventSubsystem* EventSubsystem = UMCore_EventSubsystem::Get(WorldContext))
	{
		EventSubsystem->BroadcastEvent(Event);
	}
	else
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("Could not find Event Subsystem for broadcasting"));
	}
}

void UMCore_EventBlueprintLibrary::BroadcastSimple(UObject* WorldContext, const FGameplayTag& EventTag, 
	EMCore_EventScope Scope, const FString& Parameter)
{
	if (UMCore_EventSubsystem* EventSubsystem = UMCore_EventSubsystem::Get(WorldContext))
	{
		EventSubsystem->BroadcastSimple(EventTag, Scope, Parameter);
	}
	else
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("Could not find Event Subsystem for broadcasting"));
	}
}

void UMCore_EventBlueprintLibrary::BroadcastUIEvent(UObject* WorldContext, const FGameplayTag& EventTag, const FString& Parameter)
{
	if (UMCore_EventSubsystem* EventSubsystem = UMCore_EventSubsystem::Get(WorldContext))
	{
		EventSubsystem->BroadcastUIEvent(EventTag, Parameter);
	}
}

void UMCore_EventBlueprintLibrary::BroadcastPlayerEvent(UObject* WorldContext, const FGameplayTag& EventTag, const FString& Parameter)
{
	if (UMCore_EventSubsystem* EventSubsystem = UMCore_EventSubsystem::Get(WorldContext))
	{
		EventSubsystem->BroadcastPlayerEvent(EventTag, Parameter);
	}
}

void UMCore_EventBlueprintLibrary::BroadcastGameplayEvent(UObject* WorldContext, const FGameplayTag& EventTag, const FString& Parameter)
{
	if (UMCore_EventSubsystem* EventSubsystem = UMCore_EventSubsystem::Get(WorldContext))
	{
		EventSubsystem->BroadcastGameplayEvent(EventTag, Parameter);
	}
}

UMCore_EventSubsystem* UMCore_EventBlueprintLibrary::GetEventSubsystem(const UObject* WorldContext)
{
	return UMCore_EventSubsystem::Get(WorldContext);
}