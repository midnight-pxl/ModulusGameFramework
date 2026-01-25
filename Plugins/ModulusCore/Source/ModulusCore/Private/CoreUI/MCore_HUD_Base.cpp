// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUI/MCore_HUD_Base.h"

#include "CoreData/Logging/LogModulusUI.h"
#include "CoreUI/Widgets/MCore_PrimaryGameLayout.h"
#include "CoreUI/MCore_UISubsystem.h"

AMCore_HUD_Base::AMCore_HUD_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UMCore_PrimaryGameLayout* AMCore_HUD_Base::GetPrimaryGameLayout() const
{
	if (const UMCore_UISubsystem* UISubsystem = GetUISubsystem())
	{
		return UISubsystem->GetPrimaryGameLayout();
	}
	return nullptr;
}

bool AMCore_HUD_Base::HasValidPrimaryGameLayout() const
{
	return IsValid(GetPrimaryGameLayout());
}

UMCore_UISubsystem* AMCore_HUD_Base::GetUISubsystem() const
{
	if (const APlayerController* PlayerController = GetOwningPlayerController())
	{
		if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			return LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
		}
	}
	return nullptr;
}

void AMCore_HUD_Base::DrawHUD()
{
	Super::DrawHUD();
	
	// Override in subclass for canvas-based drawing:
	// - Debug visualization
	// - Crosshairs that need sub-frame precision
	// - Performance overlays
	// 
	// For widget-based UI, use UMCore_UISubsystem instead.
}

