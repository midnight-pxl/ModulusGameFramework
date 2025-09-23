// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MCore_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MODULUSCORE_API AMCore_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* aPawn) override;
};
