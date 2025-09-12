// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreEventSystem/EventSystemData/MCore_BroadcastFunctionLibrary.h"

#include "CoreData/CoreLogging/LogModulusEvent.h"
#include "CoreEventSystem/MCore_EventSubsystem.h"
#include "CoreEventSystem/EventSystemData/MCore_EventDetails.h"
#include "CoreEventSystem/EventSystemData/MCore_EventScope.h"

bool UMCore_BroadcastFunctionLibrary::OptimizedBroadcastEvent(UObject* WorldContext, const FGameplayTag& EventTag,
	EMCore_EventScope EventScope, const FString& Parameter)
{
	if (!ValidateEventBroadcast(WorldContext, EventTag)) { return false; }

	UMCore_EventSubsystem* EventSubsystem = GetValidatedSubsystem(WorldContext);
	if (!EventSubsystem) { return false; }

	FMCore_EventData EventData(EventTag, EventScope, Parameter);
	EventSubsystem->BroadcastEvent(EventData);

	return true;
}

bool UMCore_BroadcastFunctionLibrary::ValidateEventBroadcast(UObject* WorldContext, const FGameplayTag& EventTag)
{
	if (!WorldContext)
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("Invalid WorldContext for event broadcast"));
		return false;
	}

	if (!EventTag.IsValid())
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("Invalid EventTag for broadcast"));
		return false;
	}

	return true;
}

UMCore_EventSubsystem* UMCore_BroadcastFunctionLibrary::GetValidatedSubsystem(const UObject* WorldContext)
{
	if (!WorldContext) { return nullptr; }

	UMCore_EventSubsystem* EventSubsystem = UMCore_EventSubsystem::Get(WorldContext);
	if (!EventSubsystem)
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("Could not validate ModulusCore Event Subsystem"));
		return nullptr;
	}

	return EventSubsystem;
}
