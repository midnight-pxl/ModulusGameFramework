// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/MCore_UISubsystem.h"
#include "CoreUISystem/MCore_PrimaryGameLayout.h"
#include "CoreData/CoreLogging/LogModulusUI.h"

UMCore_PrimaryGameLayout* UMCore_UISubsystem::GetPrimaryGameLayout() const
{
	if (CachedPrimaryGameLayout.IsValid())
	{
		return CachedPrimaryGameLayout.Get();
	}
    
	UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem: No PrimaryGameLayout registered for this local player"));
	return nullptr;
}

void UMCore_UISubsystem::RegisterPrimaryGameLayout(UMCore_PrimaryGameLayout* InLayout)
{
	if (!InLayout)
	{
		UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: Attempted to register null PrimaryGameLayout"));
		return;
	}

	CachedPrimaryGameLayout = InLayout;
	UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: Registered PrimaryGameLayout"));
}
