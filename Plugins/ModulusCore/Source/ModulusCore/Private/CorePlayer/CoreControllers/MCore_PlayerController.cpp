// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CorePlayer/CoreControllers/MCore_PlayerController.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

void AMCore_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("MCore.MainMenu.Default"),FoundCameras);

	if (!FoundCameras.IsEmpty())
	{
		SetViewTargetWithBlend(FoundCameras[0]);
	}
}
