// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreData/CoreLibraries/EventsLibraries/MCore_EventFunctionLibrary.h"
#include "Engine/World.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "CoreData/CoreLogging/LogModulusEvent.h"
#include "CoreData/CoreStructEnums/EventStructsEnums/MCore_EventData.h"
#include "CoreEventSystem/MCore_GlobalEventSubsystem.h"
#include "CoreEventSystem/MCore_LocalEventSubsystem.h"

void UMCore_EventFunctionLibrary::BroadcastSimpleEvent(const UObject* WorldContext, FGameplayTag EventTag,
	EMCore_EventScope EventScope)
{
	TMap<FString, FString> EmptyParams;
	BroadcastEvent(WorldContext, EventTag, EmptyParams, EventScope);
}

void UMCore_EventFunctionLibrary::BroadcastEventWithContext(const UObject* WorldContext, FGameplayTag EventTag,
	const FString& ContextID, EMCore_EventScope EventScope)
{
	if (!WorldContext || !EventTag.IsValid())
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("BroadcastEventWithContext: Invalid parameters"));
		return;
	}

	// Create event data with context ID
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

void UMCore_EventFunctionLibrary::BroadcastLocalEvent(const UObject* WorldContext, FGameplayTag EventTag,
                                                      const TMap<FString, FString>& Parameters)
{
	BroadcastEvent(WorldContext, EventTag, Parameters, EMCore_EventScope::Local);
}

void UMCore_EventFunctionLibrary::BroadcastGlobalEvent(const UObject* WorldContext, FGameplayTag EventTag,
	const TMap<FString, FString>& Parameters)
{
	BroadcastEvent(WorldContext, EventTag, Parameters, EMCore_EventScope::Global);
}

FMCore_EventData UMCore_EventFunctionLibrary::MakeEventData(FGameplayTag EventTag,
	const TMap<FString, FString>& Parameters)
{
	return FMCore_EventData(EventTag, Parameters);
}

TMap<FString, FString> UMCore_EventFunctionLibrary::MakeEventParameters(const TArray<FString>& Keys,
																		const TArray<FString>& Values)
{
	TMap<FString, FString> Params;
    
	int32 MaxIndex = FMath::Min(Keys.Num(), Values.Num());
	Params.Reserve(MaxIndex);
    
	for (int32 i = 0; i < MaxIndex; ++i)
	{
		if (!Keys[i].IsEmpty())
		{
			Params.Add(Keys[i], Values[i]);
		}
	}
    
	return Params;
}

/**
 *  parameter helpers
 */

FString UMCore_EventFunctionLibrary::GetEventContextID(const FMCore_EventData& EventData)
{
	return EventData.ContextID;
}

FString UMCore_EventFunctionLibrary::GetEventParameter(const FMCore_EventData& EventData, const FString& Key,
	const FString& DefaultValue)
{
	return EventData.GetParameter(Key, DefaultValue);
}

bool UMCore_EventFunctionLibrary::GetBoolParameter(const FMCore_EventData& EventData,
	const FString& Key,
	bool DefaultValue)
{
	FString Value = GetEventParameter(EventData, Key);
	if (Value.IsEmpty()) 
	{
		return DefaultValue;
	}
    
	// Handle common bool representations
	Value = Value.ToLower();
	return Value == TEXT("true") || Value == TEXT("1") || Value == TEXT("yes") || Value == TEXT("on");
}

int32 UMCore_EventFunctionLibrary::GetIntParameter(const FMCore_EventData& EventData, const FString& Key,
	int32 DefaultValue)
{
	FString Value = GetEventParameter(EventData, Key);
	if (Value.IsEmpty()) 
	{
		return DefaultValue;
	}
	
	return FCString::Atoi(*Value);
}

float UMCore_EventFunctionLibrary::GetFloatParameter(const FMCore_EventData& EventData, const FString& Key,
	float DefaultValue)
{
	FString Value = GetEventParameter(EventData, Key);
	if (Value.IsEmpty()) 
	{
		return DefaultValue;
	}
	
	return FCString::Atof(*Value);
}

FString UMCore_EventFunctionLibrary::GetStringParameter(const FMCore_EventData& EventData, const FString& Key,
	const FString& DefaultValue)
{
	return EventData.GetParameter(Key, DefaultValue);
}

FVector UMCore_EventFunctionLibrary::GetVectorParameter(const FMCore_EventData& EventData, const FString& Key,
	const FVector& DefaultValue)
{
	FString Value = GetEventParameter(EventData, Key);
	if (Value.IsEmpty()) 
	{
		return DefaultValue;
	}
    
	FVector Result;
	return Result.InitFromString(Value) ? Result : DefaultValue;
}

FGameplayTag UMCore_EventFunctionLibrary::GetGameplayTagParameter(const FMCore_EventData& EventData, const FString& Key,
	const FGameplayTag& DefaultValue)
{
	FString Value = GetEventParameter(EventData, Key);
	if (Value.IsEmpty()) 
	{
		return DefaultValue;
	}
    
	// Convert string back to gameplay tag
	FGameplayTag Result = FGameplayTag::RequestGameplayTag(FName(*Value), false);
	return Result.IsValid() ? Result : DefaultValue;
}

void UMCore_EventFunctionLibrary::RouteEventToSubsystem(const UObject* WorldContext, const FMCore_EventData& EventData,
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
	
	UE_LOG(LogModulusEvent, Verbose, TEXT("Routing event: %s (Scope: %s, Params: %d)"), 
		   *EventData.EventTag.ToString(), 
		   EventScope == EMCore_EventScope::Global ? TEXT("Global") : TEXT("Local"),
		   EventData.EventParams.Num());

	if (EventScope == EMCore_EventScope::Global)
	{
		// Route to global event subsystem (server authority)
		if (UMCore_GlobalEventSubsystem* GlobalSystem = GameInstance->GetSubsystem<UMCore_GlobalEventSubsystem>())
		{
			ENetMode NetMode = World->GetNetMode();
			if (NetMode == NM_Standalone || NetMode == NM_ListenServer || NetMode == NM_DedicatedServer)
			{
				// We have authority, broadcast directly
				GlobalSystem->BroadcastGlobalEvent(EventData);
			}
			else if (NetMode == NM_Client)
			{
				// Client sends to server for validation and broadcasting
				GlobalSystem->ServerBroadcastGlobalEvent(EventData);
			}
		}
		else
		{
			UE_LOG(LogModulusEvent, Error, TEXT("Failed to get GlobalEventSubsystem"));
		}
	}
	else // Local scoped event
	{
		// Route to local event subsystem (client-side only)
		if (ULocalPlayer* LocalPlayer = GameInstance->GetFirstGamePlayer())
		{
			if (UMCore_LocalEventSubsystem* LocalSystem = LocalPlayer->GetSubsystem<UMCore_LocalEventSubsystem>())
			{
				LocalSystem->BroadcastLocalEvent(EventData);
			}
			else
			{
				UE_LOG(LogModulusEvent, Error, TEXT("Failed to get LocalEventSubsystem"));
			}
		}
	}
}
