// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CorePlayer/MCore_PlayerController.h"

#include "Net/UnrealNetwork.h"
#include "CoreData/Logging/LogModulusUI.h"


AMCore_PlayerController::AMCore_PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMCore_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMCore_PlayerController, PlayerStateTags);
}

void AMCore_PlayerController::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = PlayerStateTags;
}

void AMCore_PlayerController::AddPlayerStateTag(FGameplayTag Tag)
{
	if (!Tag.IsValid())
	{
		UE_LOG(LogModulusUI, Log, TEXT("AddPlayerStateTag: Invalid tag"));
		return;
	}
	
	if (PlayerStateTags.IsValid())
	{
		PlayerStateTags.AddTag(Tag);
		UE_LOG(LogModulusUI, Log, TEXT("AddPlayerStateTag: Added tag %s"),
			*Tag.ToString());
	}
}

void AMCore_PlayerController::RemovePlayerStateTag(FGameplayTag Tag)
{
	if (!Tag.IsValid())
	{
		UE_LOG(LogModulusUI, Log, TEXT("RemovePlayerStateTag: Invalid Tag"));
		return;
	}
	
	if (PlayerStateTags.IsValid())
	{
		PlayerStateTags.RemoveTag(Tag);
		UE_LOG(LogModulusUI, Log, TEXT("RemovePlayerStateTag: Removed tag %s"),
			*Tag.ToString())
	}
}

bool AMCore_PlayerController::HasPlayerStateTag(FGameplayTag Tag) const
{
	return PlayerStateTags.IsValid() && PlayerStateTags.HasTag(Tag);
}

bool AMCore_PlayerController::HasAnyPlayerStateTags(const FGameplayTagContainer& BlockTags) const
{
	return PlayerStateTags.IsValid() && PlayerStateTags.HasAny(BlockTags);
}

void AMCore_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMCore_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!IsLocalController()) return;
	
}

void AMCore_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AMCore_PlayerController::ToggleInGameMenu()
{
}
