// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CorePlayer/CoreControllers/MCore_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraActor.h"
#include "CoreData/CoreLogging/LogModulusSettings.h"
#include "CoreUISystem/MCore_PrimaryGameLayout.h"
#include "CoreUISystem/MCore_UISubsystem.h"
#include "Kismet/GameplayStatics.h"

void AMCore_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMCore_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!IsLocalController()) return;

	
}

void AMCore_PlayerController::OnInGameMenuToggle()
{
	if (UMCore_UISubsystem* UISubsystem = GetLocalPlayer()->GetSubsystem<UMCore_UISubsystem>())
	{
		if (UMCore_PrimaryGameLayout* GameLayout = UISubsystem->GetPrimaryGameLayout())
		{
			UCommonActivatableWidgetStack* LayerStack = GameLayout->MCore_GameMenuLayer;

			if (LayerStack->GetActiveWidget()) { LayerStack->PopWidget(); }
			else
			{
				LayerStack->PushWidget(MenuHu)
			}
		}
	}
}