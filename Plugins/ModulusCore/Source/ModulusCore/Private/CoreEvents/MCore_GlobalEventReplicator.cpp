// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreEvents/MCore_GlobalEventReplicator.h"
#include "CoreEvents/MCore_GlobalEventSubsystem.h"
#include "CoreData/Logging/LogModulusEvent.h"
#include "CoreData/Types/Events/MCore_EventData.h"

UMCore_GlobalEventReplicator::UMCore_GlobalEventReplicator()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UMCore_GlobalEventReplicator::BeginPlay()
{
	Super::BeginPlay();

	/** Register w/ GlobalEventSubsystem */
	if (UMCore_GlobalEventSubsystem* Subsystem = GetEventSubsystem())
	{
		Subsystem->RegisterEventReplicator(this);
		UE_LOG(LogModulusEvent, Log, TEXT("GlobalEventReplicator registered with GlobalEventSubsystem"));
	}
	else
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("GlobalEventReplicator: Could not find GlobalEventSubsystem"));
	}
}

void UMCore_GlobalEventReplicator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	/** Unregister from the GlobalEventSubsystem */
	if (UMCore_GlobalEventSubsystem* Subsystem = GetEventSubsystem())
	{
		Subsystem->UnregisterEventReplicator(this);
	}
	
	CachedSubsystem.Reset();
	
	Super::EndPlay(EndPlayReason);
}

void UMCore_GlobalEventReplicator::RequestBroadcast(const FMCore_EventData& EventData)
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogModulusEvent, Warning, TEXT("RequestBroadcast: No owner actor"));
		return;
	}
	
	if (Owner->HasAuthority())
	{
		/** Server/Standalone: Deliver locally + multicast to Clients */
		if (UMCore_GlobalEventSubsystem* Subsystem = GetEventSubsystem())
		{
			Subsystem->DeliverToLocalListeners(EventData);
		}
		MulticastToClients(EventData);
	}
	else // Client-Only
	{
		ServerRequestBroadcast(EventData);
	}
}

void UMCore_GlobalEventReplicator::ServerRequestBroadcast_Implementation(const FMCore_EventData& EventData)
{
	UE_LOG(LogModulusEvent, Verbose, TEXT("Server received Broadcast request: %s"),
		*EventData.EventTag.ToString());
	
	/** Server has authority -- deliver locally and multicast */
	if (UMCore_GlobalEventSubsystem* Subsystem = GetEventSubsystem())
	{
		Subsystem->DeliverToLocalListeners(EventData);
	}
	MulticastToClients(EventData);
}

bool UMCore_GlobalEventReplicator::ServerRequestBroadcast_Validate(const FMCore_EventData& EventData)
{
	if (UMCore_GlobalEventSubsystem* Subsystem = GetEventSubsystem())
	{
		return Subsystem->ValidateEventRequest(EventData);
	}
	/** No Subsystem Active -- reject */
	return false;
}

void UMCore_GlobalEventReplicator::MulticastToClients_Implementation(const FMCore_EventData& EventData)
{
	AActor* Owner = GetOwner();
	
	/** Skip on Server -- Delivered earlier in Broadcast flow */
	if (Owner && Owner->HasAuthority()) { return; }
	
	if (UMCore_GlobalEventSubsystem* Subsystem = GetEventSubsystem())
	{
		Subsystem->DeliverToLocalListeners(EventData);
		UE_LOG(LogModulusEvent, Verbose, TEXT("Client received multicast: %s"),
			*EventData.EventTag.ToString());
	}
}

UMCore_GlobalEventSubsystem* UMCore_GlobalEventReplicator::GetEventSubsystem() const
{
	if (CachedSubsystem.IsValid()) { return CachedSubsystem.Get(); }
	
	/** Find subsystem + cache */
	UWorld* World = GetWorld();
	if (!World) { return nullptr; }
	
	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance) { return nullptr; }
	
	UMCore_GlobalEventSubsystem* Subsystem = GameInstance->GetSubsystem<UMCore_GlobalEventSubsystem>();
	CachedSubsystem = Subsystem;
	return Subsystem;
}

