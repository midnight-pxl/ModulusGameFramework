// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUISystem/MCore_UISubsystem.h"

#include "GameplayTagContainer.h"
#include "Engine/LocalPlayer.h"

#include "CoreData/CoreLogging/LogModulusUI.h"
#include "CoreUISystem/CoreWidgets/MCore_GameMenuHub.h"
#include "CoreData/CoreDevSettings/MCore_CommonUISettings.h"
#include "CoreUISystem/CoreWidgets/MCore_PrimaryGameLayout.h"
#include "CoreData/CoreDataAssets/UIDAs/MCore_DA_UITheme_Base.h"


void UMCore_UISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadWidgetClasses();

	UE_LOG(LogModulusUI, Log, TEXT("MCore_UISubsystem Initialized (awaiting PrimaryGameLayout registration from HUD)"));
}

void UMCore_UISubsystem::Deinitialize()
{
	UE_LOG(LogModulusUI, Log, TEXT("UISubsystem::Deinitialize - Cleaning up"));

	/** Clear PrimaryGameLayout reference (HUD owns) */
	CachedPrimaryGameLayout.Reset();
	
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
	if (!IsValid(InLayout))
	{
		UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem: Attempted to register null PrimaryGameLayout"));
		return false;
	}

	if (CachedPrimaryGameLayout.IsValid())
	{
		if (CachedPrimaryGameLayout.Get() == InLayout)
		{
			UE_LOG(LogModulusUI, Verbose, TEXT("UISubsystem: PrimaryGameLayout '%s' already registered"), 
				*GetNameSafe(InLayout));
			return true;
		}
	
	// Different layout - warn but allow replacement (may happen during level transitions)
	UE_LOG(LogModulusUI, Warning, 
		TEXT("UISubsystem: Replacing existing PrimaryGameLayout '%s' with '%s'"),
		*GetNameSafe(CachedPrimaryGameLayout.Get()), *GetNameSafe(InLayout));
	}

	CachedPrimaryGameLayout = InLayout;
	UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: Registered PrimaryGameLayout '%s'"), *GetNameSafe(InLayout));

	// If MenuHub was created before layout, it may need to rebuild
	if (CachedMenuHub.IsValid())
	{
		CachedMenuHub->RebuildTabBar();
	}
	
	return true;
}

void UMCore_UISubsystem::UnregisterPrimaryGameLayout()
{
	if (CachedPrimaryGameLayout.IsValid())
	{
		UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: Unregistered PrimaryGameLayout '%s'"), 
			*GetNameSafe(CachedPrimaryGameLayout.Get()));
	}
	
	CachedPrimaryGameLayout.Reset();
}

UMCore_GameMenuHub* UMCore_UISubsystem::GetOrCreateMenuHub()
{
	/** Return cached instance if valid */
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

	/** Create MenuHub widget */
	CachedMenuHub = CreateWidget<UMCore_GameMenuHub>(PlayerController, MenuHubClass);
	if (!CachedMenuHub.IsValid())
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("UISubsystem: CreateWidget failed for MenuHub"));
		return nullptr;
	}

	/** Build tab bar with registered screens */
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
	
	/** Check for duplicate registration */
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

	/** Create registration entry */
	FMCore_MenuTab NewTab;
	NewTab.TabID = TabID;
	NewTab.ScreenWidgetClass = ScreenWidgetClass;
	NewTab.Priority = Priority;
	NewTab.TabIcon = TabIcon;
    
	/** Insert sorted by priority (lower priority = earlier in tab list) */
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

	/** If MenuHub already created, rebuild tab bar to show new screen */
	if (CachedMenuHub.IsValid())
	{
		CachedMenuHub.Get()->RebuildTabBar();
	}
}

bool UMCore_UISubsystem::UnregisterMenuScreen(FGameplayTag TabID)
{
	if (!TabID.IsValid())
	{
		UE_LOG(LogModulusUI, Warning, 
			TEXT("UISubsystem::UnregisterMenuScreen: Invalid TabID"));
		return false;
	}

	const int32 RemovedCount = RegisteredMenuScreens.RemoveAll([TabID](const FMCore_MenuTab& Tab)
	{
		return Tab.TabID == TabID;
	});

	if (RemovedCount > 0)
	{
		UE_LOG(LogModulusUI, Log,
			TEXT("UISubsystem::UnregisterMenuScreen: '%s' unregistered (Remaining: %d)"),
			*TabID.ToString(), RegisteredMenuScreens.Num());

		// Rebuild MenuHub if it exists
		if (CachedMenuHub.IsValid())
		{
			CachedMenuHub->RebuildTabBar();
		}
		return true;
	}

	UE_LOG(LogModulusUI, Warning,
		TEXT("UISubsystem::UnregisterMenuScreen: Tab '%s' not found"),
		*TabID.ToString());
	return false;
}

void UMCore_UISubsystem::RebuildMenuHubTabBar()
{
	if (CachedMenuHub.IsValid())
	{
		CachedMenuHub->RebuildTabBar();
	}
}

UMCore_DA_UITheme_Base* UMCore_UISubsystem::GetActiveTheme() const
{
	const UMCore_CommonUISettings* UISettings = UMCore_CommonUISettings::Get();
	if (UISettings && !UISettings->CurrentThemeAsset.IsNull())
	{
		return UISettings->CurrentThemeAsset.LoadSynchronous();
	}
	return nullptr;	
}

void UMCore_UISubsystem::LoadWidgetClasses()
{
	// Hard-coded defaults for v1.0 - no project settings complexity yet
	MenuHubClass = UMCore_GameMenuHub::StaticClass();

	if (!MenuHubClass)
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("UISubsystem: MenuHubClass is nullptr - check class exists"));
	}
    
	UE_LOG(LogModulusUI, Verbose, 
		TEXT("UISubsystem: Widget classes loaded - MenuHub: %s"),
		MenuHubClass ? TEXT("OK") : TEXT("FAIL"));
}