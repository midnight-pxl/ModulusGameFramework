// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreEventSystem/MCore_EventFunctionLibrary.h"
#include "Engine/World.h"
#include "CoreData/CoreLogging/LogModulusEvent.h"
#include "CoreEventSystem/MCore_EventData.h"
#include "CoreEventSystem/MCore_GlobalEventSubsystem.h"
#include "CoreEventSystem/MCore_LocalEventSubsystem.h"

void UMCore_EventFunctionLibrary::BroadcastEvent(const UObject* WorldContext,
	FGameplayTag EventTag,
	EMCore_EventScope EventScope,
	const TMap<FString, FString>& EventParams)
{
	if (!WorldContext || !EventTag.IsValid())
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("BroadcastEvent: Invalid parameters (WorldContext: %s, Tag: %s)"),
			WorldContext ? TEXT("Valid") : TEXT("NULL"), *EventTag.ToString());
		return;
	}

	const UWorld* World = WorldContext->GetWorld();
	if (!World)
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("BroadcastEvent: No valid world context"));
		return;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("BroadcastEvent: No valid game instance"));
		return;
	}
	
	FMCore_EventData EventData(EventTag, EventParams);

	UE_LOG(LogModulusEvent, Verbose, TEXT("Broadcasting event: %s (Scope: %s, Params: %d)"), 
	   *EventTag.ToString(), 
	   EventScope == EMCore_EventScope::Global ? TEXT("Global") : TEXT("Local"),
	   EventParams.Num());

	if (EventScope == EMCore_EventScope::Global)
	{
		// Route to global event subsystem (server authority)
		if (UMCore_GlobalEventSubsystem* GlobalEventSys = GameInstance->GetSubsystem<UMCore_GlobalEventSubsystem>())
		{
			ENetMode NetMode = World->GetNetMode();
			if (NetMode == NM_Standalone || NetMode == NM_ListenServer || NetMode == NM_DedicatedServer)
			{
				GlobalEventSys->BroadcastGlobalEvent(EventData);
			}
			else if (NetMode == NM_Client)
			{
				GlobalEventSys->ServerBroadcastGlobalEvent(EventData);
			}
			else
			{
				UE_LOG(LogModulusEvent, Error, TEXT("Failed to get GlobalEventSubsystem"));
			}
		}
	}
	else // Local scoped event
	{
		// Route to local event subsystem (client-side only)
		if (ULocalPlayer* LocalPlayer = GameInstance->GetFirstGamePlayer())
		{
			if (UMCore_LocalEventSubsystem* LocalEventSys = LocalPlayer->GetSubsystem<UMCore_LocalEventSubsystem>())
			{
				LocalEventSys->BroadcastLocalEvent(EventData);
			}
			else
			{
				UE_LOG(LogModulusEvent, Error, TEXT("Failed to get LocalEventSubsystem"));
			}
		}
	}
}

void UMCore_EventFunctionLibrary::BroadcastSimpleEvent(const UObject* WorldContext, FGameplayTag EventTag,
	EMCore_EventScope EventScope)
{
	TMap<FString, FString> EmptyParams;
	BroadcastEvent(WorldContext, EventTag, EventScope, EmptyParams);
}

void UMCore_EventFunctionLibrary::BroadcastLocalEvent(const UObject* WorldContext, FGameplayTag EventTag,
                                                      const TMap<FString, FString>& Parameters)
{
	BroadcastEvent(WorldContext, EventTag, EMCore_EventScope::Local, Parameters);
}

void UMCore_EventFunctionLibrary::BroadcastGlobalEvent(const UObject* WorldContext, FGameplayTag EventTag,
	const TMap<FString, FString>& Parameters)
{
	BroadcastEvent(WorldContext, EventTag, EMCore_EventScope::Global, Parameters);
}

FMCore_EventData UMCore_EventFunctionLibrary::MakeEventData(FGameplayTag EventTag,
                                                            const TMap<FString, FString>& Parameters)
{
	return FMCore_EventData(EventTag, Parameters);
}

TMap<FString, FString> UMCore_EventFunctionLibrary::MakeEmptyParamMap()
{
	return TMap<FString, FString>();
}

/**
 * Type conversion parameter helpers
 */
bool UMCore_EventFunctionLibrary::GetBoolParameter(const FMCore_EventData& EventData,
	const FString& Key,
	bool DefaultValue)
{
	FString Value = GetStringParameter(EventData, Key);
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
	FString Value = GetStringParameter(EventData, Key);
	if (Value.IsEmpty()) 
	{
		return DefaultValue;
	}
	
	return FCString::Atoi(*Value);
}

float UMCore_EventFunctionLibrary::GetFloatParameter(const FMCore_EventData& EventData, const FString& Key,
	float DefaultValue)
{
	FString Value = GetStringParameter(EventData, Key);
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
	FString Value = GetStringParameter(EventData, Key);
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
	FString Value = GetStringParameter(EventData, Key);
	if (Value.IsEmpty()) 
	{
		return DefaultValue;
	}
    
	// Convert string back to gameplay tag
	FGameplayTag Result = FGameplayTag::RequestGameplayTag(FName(*Value), false);
	return Result.IsValid() ? Result : DefaultValue;
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
