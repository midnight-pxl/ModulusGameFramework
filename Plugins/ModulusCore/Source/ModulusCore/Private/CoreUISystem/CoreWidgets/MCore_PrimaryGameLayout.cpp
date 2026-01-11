// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/CoreWidgets/MCore_PrimaryGameLayout.h"

#include "CoreData/CoreLogging/LogModulusUI.h"
#include "CoreUISystem/MCore_UISubsystem.h"

UMCore_PrimaryGameLayout::UMCore_PrimaryGameLayout(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMCore_PrimaryGameLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	UE_LOG(LogModulusUI, Log, TEXT("PrimaryGameLayout initialized successfully with all 4 layers"));

	/** Register with UISubsystem for cross-plugin access */
	if (APlayerController* ControllerRef = GetOwningPlayer())
	{
		if (ULocalPlayer* LocalPlayer = ControllerRef->GetLocalPlayer())
		{
			if (UMCore_UISubsystem* UISubsystem = LocalPlayer->GetSubsystem<UMCore_UISubsystem>())
			{
				UISubsystem->RegisterPrimaryGameLayout(this);
			}
		}
	}
}