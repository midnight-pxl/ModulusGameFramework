// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CorePlayer/MCore_PlayerController.h"
#include "CoreUI/Widgets/MCore_PrimaryGameLayout.h"
#include "CoreData/Tags/MCore_SettingsTags.h"
#include "CoreData\Logging\LogModulusUI.h"
#include "CoreUI/Widgets/Primitives/MCore_ActivatableBase.h"
#include "Net/UnrealNetwork.h"

AMCore_PlayerController::AMCore_PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMCore_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMCore_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AMCore_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AMCore_PlayerController::ToggleInGameMenu()
{
}















