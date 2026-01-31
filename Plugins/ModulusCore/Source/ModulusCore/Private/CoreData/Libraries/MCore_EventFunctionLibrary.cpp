// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreData/Libraries/MCore_EventFunctionLibrary.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "CoreData/Logging/LogModulusEvent.h"
#include "CoreData/Types/Events/MCore_EventData.h"
#include "CoreEvents/MCore_GlobalEventSubsystem.h"
#include "CoreEvents/MCore_LocalEventSubsystem.h"

//~ Start of Broadcast Functions

void UMCore_EventFunctionLibrary::BroadcastSimpleEvent(const UObject* WorldContext,
	FGameplayTag EventTag,
	EMCore_EventScope EventScope)
{
	if (!WorldContext || !EventTag.IsValid())
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("BroadcastSimpleEvent: Invalid parameters (WorldContext: %s, Tag: %s)"),
			WorldContext ? TEXT("Valid") : TEXT("NULL"), *EventTag.ToString());
		return;
	}
	
	FMCore_EventData EventData(EventTag);
	RouteEventToSubsystem(WorldContext, EventData, EventScope);
}

void UMCore_EventFunctionLibrary::BroadcastEventWithContext(const UObject* WorldContext,
	FGameplayTag EventTag,
	const FString& ContextID,
	EMCore_EventScope EventScope)
{
	if (!WorldContext || !EventTag.IsValid())
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("BroadcastEventWithContext: Invalid parameters (WorldContext: %s, Tag: %s)"),
			WorldContext ? TEXT("Valid") : TEXT("NULL"), *EventTag.ToString());
		return;
	}

	/** Create event data with context ID */
	FMCore_EventData EventData(EventTag, ContextID);
	RouteEventToSubsystem(WorldContext, EventData, EventScope);
}

void UMCore_EventFunctionLibrary::BroadcastEvent(const UObject* WorldContext,
	FGameplayTag EventTag,
	const TMap<FString, FString>& EventParams,
	EMCore_EventScope EventScope)
{
	if (!WorldContext || !EventTag.IsValid())
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("BroadcastEvent: Invalid parameters (WorldContext: %s, Tag: %s)"),
			   WorldContext ? TEXT("Valid") : TEXT("NULL"), *EventTag.ToString());
		return;
	}

	FMCore_EventData EventData(EventTag, EventParams);
	RouteEventToSubsystem(WorldContext, EventData, EventScope);
}

//~ End of Broadcast Functions

//~ Start of Parameter Accessors

FString UMCore_EventFunctionLibrary::GetEventContextID(const FMCore_EventData& EventData)
{
	return EventData.ContextID;
}

FString UMCore_EventFunctionLibrary::GetEventParameter(const FMCore_EventData& EventData,
	const FString& Key,
	const FString& DefaultValue)
{
	return EventData.GetParameter(Key, DefaultValue);
}

bool UMCore_EventFunctionLibrary::GetBoolParameter(const FMCore_EventData& EventData,
	const FString& Key,
	bool DefaultValue)
{
	const FString InValue = EventData.GetParameter(Key);
	if (InValue.IsEmpty()) { return DefaultValue; }
	
	const FString ParamValue = InValue.ToLower();
	return ParamValue == TEXT("true") || ParamValue == TEXT("1")
	|| ParamValue == TEXT("on") || ParamValue == TEXT("yes");
}

int32 UMCore_EventFunctionLibrary::GetIntParameter(const FMCore_EventData& EventData,
	const FString& Key,
	int32 DefaultValue)
{
	const FString InValue = EventData.GetParameter(Key);
	if (InValue.IsEmpty()) { return DefaultValue; }
	
	return FCString::Atoi(*InValue);
}

float UMCore_EventFunctionLibrary::GetFloatParameter(const FMCore_EventData& EventData,
	const FString& Key,
	float DefaultValue)
{
	FString InValue = EventData.GetParameter(Key);
	if (InValue.IsEmpty()) { return DefaultValue; }
	
	return FCString::Atof(*InValue);
}

//~ End of Parameter Accessors

//~ Subsystem Internal Routing

void UMCore_EventFunctionLibrary::RouteEventToSubsystem(const UObject* WorldContext,
	const FMCore_EventData& EventData,
	EMCore_EventScope EventScope)
{
	const UWorld* World = WorldContext->GetWorld();
	if (!World)
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("RouteEventToSubsystem: No valid world context"));
		return;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("RouteEventToSubsystem: No valid game instance"));
		return;
	}
	
	UE_LOG(LogModulusEvent, Verbose, TEXT("Routing event: %s (Scope: %s)"), 
		   *EventData.EventTag.ToString(), 
		   EventScope == EMCore_EventScope::Global ? TEXT("Global") : TEXT("Local"));

	if (EventScope == EMCore_EventScope::Global)
	{
		/** Global event: Subsystem handles Authority, Replicator routing, and fallback */
		if (UMCore_GlobalEventSubsystem* GlobalSystem = GameInstance->GetSubsystem<UMCore_GlobalEventSubsystem>())
		{
			GlobalSystem->BroadcastGlobalEvent(EventData);
		}
		else
		{
			UE_LOG(LogModulusEvent, Error, TEXT("Failed to get GlobalEventSubsystem"));
		}
	}
	else
	{
		/** Local event: Route to LocalPlayerSubsystem */
		if (ULocalPlayer* LocalPlayer = GameInstance->GetFirstGamePlayer())
		{
			if (UMCore_LocalEventSubsystem* LocalSystem = LocalPlayer->GetSubsystem<UMCore_LocalEventSubsystem>())
			{
				LocalSystem->BroadcastLocalEvent(EventData);
			}
			else
			{
				UE_LOG(LogModulusEvent, Error, TEXT("RouteEventToSubsystem: Failed to get LocalEventSubsystem"));
			}
		}
		else
		{
			UE_LOG(LogModulusEvent, Warning, TEXT("RouteEventToSubsystem: No LocalPlayer found for event"));
		}
	}
}
