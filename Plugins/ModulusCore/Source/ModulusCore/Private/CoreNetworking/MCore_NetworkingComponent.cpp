// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreNetworking/MCore_NetworkingComponent.h"

#include "CoreData/Settings/MCore_NetworkingSettings.h"
#include "CoreData/Logging/LogModulusNetworking.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

/** TODO - Uncomment if using Iris Replication System (Remove line)
 * #include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
 * Remove line */

UMCore_NetworkingComponent::UMCore_NetworkingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UMCore_NetworkingComponent::BeginPlay()
{
	Super::BeginPlay();
	DetectNetworkingSystem();
	OnNetworkInitialized();

	UE_LOG(LogModulusNetworking, Verbose, TEXT ("Modulus networked component initialized: %s "
		"(Authority: %s, Iris: %s)"), *GetClass()->GetName(),
		HasNetworkAuthority() ? TEXT("Yes") : TEXT("No"),
		bIrisDetected ? TEXT("Yes") : TEXT("No"));
	
}

void UMCore_NetworkingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnNetworkShutdown();
	Super::EndPlay(EndPlayReason);
}

template <typename TOperation>
bool UMCore_NetworkingComponent::ExecuteWithAuthority(TOperation&& Operation, bool bRequireServerAuthority)
{
	if (bRequireServerAuthority && !HasNetworkAuthority())
	{
		UE_LOG(LogModulusNetworking, Warning, TEXT("Attempted server operation without Authority: %s"),
			*GetClass()->GetName());

		return false;
	}
	return Operation();
}

void UMCore_NetworkingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ComponentNetworkID);
}

void UMCore_NetworkingComponent::DetectNetworkingSystem()
{
	bIrisDetected = false;
	
	const UMCore_NetworkingSettings* NetSettings = GetDefault<UMCore_NetworkingSettings>();
	if (!NetSettings || !NetSettings->bUseIrisReplication) { return; }

	/** TODO - Uncomment if using Iris Replication System (Remove line)
	bIrisDetected = IsIrisAvailable();
	* Remove line */
	
	if (NetSettings->bUseIrisReplication && !bIrisDetected)
	{
		UE_LOG(LogModulusNetworking, Warning, TEXT(
			"Iris replication enabled in settings but not available in this project. "
					"Ensure Target.cs has bWithIris = true"))
	}
}

bool UMCore_NetworkingComponent::IsUsingIrisReplication() const
{
	return bIrisDetected;
}

bool UMCore_NetworkingComponent::IsIrisAvailable() const
{
	/** TODO - Uncomment if using Iris Replication System (Remove line)
	* Remove line
	const FModuleManager& ModuleManager = FModuleManager::Get();
	if (ModuleManager.IsModuleLoaded(TEXT("IrisCore"))) { return true; }
	
	UClass* IrisClass = FindFirstObject<UClass>(TEXT("IrisReplicationSystem"),
		EFindFirstObjectOptions::None, ELogVerbosity::NoLogging);
	if (IrisClass) { return true; }

	if (const UWorld* UWorld = GetWorld())
	{
		if (const UNetDriver* NetDriver = UWorld->GetNetDriver())
		{
			const FString ClassName = NetDriver->GetClass()->GetName();
			if (ClassName.Contains(TEXT("Iris")))
			{
				return true;
			}
		}
	}
	* Remove line */
	return false;
}

/** TODO - Uncomment if using Iris Replication System (Remove line)
void UMCore_NetworkingComponent::RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context,
	UE::Net::EFragmentRegistrationFlags RegistrationFlags)
{
	const UMCore_NetworkingSettings* NetSettings = GetDefault<UMCore_NetworkingSettings>();
	if (!NetSettings->bUseIrisReplication)
	{
		return;
	}

	UE::Net::FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
	UE_LOG(LogModulusNetworking, Verbose, TEXT("Registered Iris replication fragments for %s"),
		*GetClass()->GetName());
}
* Remove line */

void UMCore_NetworkingComponent::ForceNetUpdate()
{
	if (!HasNetworkAuthority()) { return; }

	if (AActor* Owner = GetOwner()) { Owner->ForceNetUpdate(); }
}

