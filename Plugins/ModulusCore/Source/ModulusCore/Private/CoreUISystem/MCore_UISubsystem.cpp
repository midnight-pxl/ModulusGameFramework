// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUISystem/MCore_UISubsystem.h"

#include "GameplayTagContainer.h"
#include "CoreUISystem/MCore_PrimaryGameLayout.h"
#include "CoreUISystem/CoreWidgets/MCore_GameMenuHub.h"

#include "CommonUI/Public/CommonTabListWidgetBase.h"
#include "Engine/LocalPlayer.h"
#include "CoreData/CoreLogging/LogModulusUI.h"

void UMCore_UISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadWidgetClasses();

	UE_LOG(LogModulusUI, Log, TEXT("MCore_UISubsystem Initialized"));
}

void UMCore_UISubsystem::Deinitialize()
{
	UE_LOG(LogModulusUI, Log, TEXT("UISubsystem::Deinitialize - Cleaning up"));
	
	// Clean up cached widgets
	if (CachedPrimaryGameLayout.IsValid())
	{
		CachedPrimaryGameLayout->RemoveFromParent();
		CachedPrimaryGameLayout.Reset();
	}
	
	if (CachedMenuHub.IsValid())
	{
		CachedMenuHub->RemoveFromParent();
		CachedMenuHub.Reset();
	}
	
	RegisteredMenuScreens.Empty();
	
	Super::Deinitialize();
}

UMCore_PrimaryGameLayout* UMCore_UISubsystem::GetPrimaryGameLayout() const
{
	if (CachedPrimaryGameLayout.IsValid())
	{
		return CachedPrimaryGameLayout.Get();
	}
    
	UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem: No PrimaryGameLayout registered for this local player"));
	return nullptr;
}

bool UMCore_UISubsystem::RegisterPrimaryGameLayout(UMCore_PrimaryGameLayout* InLayout)
{
	if (!InLayout)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem: Attempted to register null PrimaryGameLayout"));
		return false;
	}

	CachedPrimaryGameLayout = InLayout;
	UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: Registered PrimaryGameLayout"));
	return true;
}

UMCore_GameMenuHub* UMCore_UISubsystem::GetOrCreateMenuHub()
{
	// Return cached instance if valid
	if (CachedMenuHub.IsValid())
	{
		return CachedMenuHub.Get();
	}
    
	if (!MenuHubClass)
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("UISubsystem: Cannot create MenuHub - class not loaded"));
		return nullptr;
	}
    
	if (!CachedPrimaryGameLayout.IsValid())
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("UISubsystem: Cannot create MenuHub - PrimaryGameLayout not created"));
		return nullptr;
	}
    
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("UISubsystem: Cannot create MenuHub - no LocalPlayer"));
		return nullptr;
	}
    
	APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld());
	if (!PlayerController)
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("UISubsystem: Cannot create MenuHub - no PlayerController"));
		return nullptr;
	}
    
	// Create MenuHub widget
	CachedMenuHub = CreateWidget<UMCore_GameMenuHub>(PlayerController, MenuHubClass);
	if (!CachedMenuHub.IsValid())
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("UISubsystem: CreateWidget failed for MenuHub"));
		return nullptr;
	}
    
	// Build tab bar with registered screens
	CachedMenuHub->RebuildTabBar();
    
	UE_LOG(LogModulusUI, Log, 
		TEXT("UISubsystem: MenuHub created with %d registered screens"), 
		RegisteredMenuScreens.Num());
    
	return CachedMenuHub.Get();
}

void UMCore_UISubsystem::RegisterMenuScreen(FGameplayTag TabID,
	TSubclassOf<UCommonActivatableWidget> ScreenWidgetClass,
	int32 Priority, UTexture2D* TabIcon)
{
	if (!ScreenWidgetClass)
	{
		UE_LOG(LogModulusUI, Warning, 
			TEXT("RegisterMenuScreen: Invalid ScreenClass provided"));
		return;
	}

	if (!TabID.IsValid())
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("RegisterMenuScreen: Invalid TabID"));
		return;
	}
	
	if (RegisteredMenuScreens.ContainsByPredicate([TabID](const FMCore_MenuTab& Tab)
	 {
		 return Tab.TabID == TabID;
	 }))
	{
		UE_LOG(LogModulusUI, Warning, 
			TEXT("RegisterMenuScreen: Tab '%s' already registered, skipping duplicate"),
			*TabID.ToString());
		return;
	}
    
	// Create registration entry
	FMCore_MenuTab NewTab;
	NewTab.TabID = TabID;
	NewTab.ScreenWidgetClass = ScreenWidgetClass;
	NewTab.Priority = Priority;
	NewTab.TabIcon = TabIcon;
    
	int32 InsertIndex = 0;
	for (; InsertIndex < RegisteredMenuScreens.Num(); ++InsertIndex)
	{
		if (RegisteredMenuScreens[InsertIndex].Priority > Priority)
		{
			break;
		}
	}
	RegisteredMenuScreens.Insert(NewTab, InsertIndex);
    
	UE_LOG(LogModulusUI, Log, 
		TEXT("RegisterMenuScreen: %s registered at priority %d (Total: %d)"),
		*NewTab.GetDisplayName().ToString(), Priority, RegisteredMenuScreens.Num());
    
	// If MenuHub already created, rebuild tab bar to show new screen
	if (CachedMenuHub.IsValid())
	{
		CachedMenuHub.Get()->RebuildTabBar();
	}
}

void UMCore_UISubsystem::RebuildTabBar()
{
}

void UMCore_UISubsystem::LoadWidgetClasses()
{
	// Hard-coded defaults - no project settings complexity for v1.0
	PrimaryGameLayoutClass = UMCore_PrimaryGameLayout::StaticClass();
	MenuHubClass = UMCore_GameMenuHub::StaticClass();
    
	// Validation with actionable error messages
	if (!PrimaryGameLayoutClass)
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("UISubsystem: PrimaryGameLayoutClass is nullptr - check class exists"));
	}
    
	if (!MenuHubClass)
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("UISubsystem: MenuHubClass is nullptr - check class exists"));
	}
    
	UE_LOG(LogModulusUI, Verbose, 
		TEXT("UISubsystem: Classes loaded - Layout:%s MenuHub:%s"),
		PrimaryGameLayoutClass ? TEXT("OK") : TEXT("FAIL"),
		MenuHubClass ? TEXT("OK") : TEXT("FAIL"));
}

void UMCore_UISubsystem::CreatePrimaryGameLayout()
{
	if (!PrimaryGameLayoutClass)
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("UISubsystem: Cannot create layout - class not loaded"));
		return;
	}
    
	ULocalPlayer* LP = GetLocalPlayer();
	if (!LP)
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("UISubsystem: Cannot create layout - no LocalPlayer"));
		return;
	}
    
	APlayerController* PC = LP->GetPlayerController(GetWorld());
	if (!PC)
	{
		UE_LOG(LogModulusUI, Warning, 
			TEXT("UISubsystem: PlayerController not ready, delaying layout creation"));
        
		// Retry on next tick - PlayerController may not exist yet during Initialize()
		FTSTicker::GetCoreTicker().AddTicker(
			FTickerDelegate::CreateUObject(this, &UMCore_UISubsystem::RetryLayoutCreation),
			0.1f
		);
		return;
	}
    
	// Create layout widget
	CachedPrimaryGameLayout = CreateWidget<UMCore_PrimaryGameLayout>(PC, PrimaryGameLayoutClass);
	if (!CachedPrimaryGameLayout.IsValid())
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("UISubsystem: CreateWidget failed for PrimaryGameLayout"));
		return;
	}
    
	// Add to viewport at Z-order 0 (base UI layer)
	CachedPrimaryGameLayout->AddToViewport(0);
    
	UE_LOG(LogModulusUI, Log, 
		TEXT("UISubsystem: PrimaryGameLayout created and added to viewport"));
}

void UMCore_UISubsystem::RebuildMenuHub()
{
	if (CachedMenuHub.IsValid()) { CachedMenuHub->RebuildTabBar(); }
}

bool UMCore_UISubsystem::RetryLayoutCreation(float DeltaTime)
{
	if (!CachedPrimaryGameLayout.IsValid())
	{
		CreatePrimaryGameLayout();
	}
    
	// Return false to stop ticker once created
	return CachedPrimaryGameLayout == nullptr;
}

