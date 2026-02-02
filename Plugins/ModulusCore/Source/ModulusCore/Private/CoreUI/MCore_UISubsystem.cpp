// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUI/MCore_UISubsystem.h"

#include "GameplayTagContainer.h"
#include "Engine/LocalPlayer.h"

#include "CoreData/Logging/LogModulusUI.h"
#include "CoreData/DevSettings/MCore_CoreSettings.h"
#include "CoreData/Tags/MCore_UILayerTags.h"
#include "CoreUI/Widgets/MCore_GameMenuHub.h"
#include "CoreUI/Widgets/MCore_PrimaryGameLayout.h"
#include "CoreData/Assets/UI/Themes/MCore_PDA_UITheme_Base.h"

//~ Start of USubsystem Interface

void UMCore_UISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (IsRunningDedicatedServer())
	{
		UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: Skipping initialization on dedicated server"));
		return;
	}

	LoadWidgetClasses();
	CreatePrimaryGameLayout();
	
	/** Load Theming for widget creation */
	const UMCore_CoreSettings* DevSettings = UMCore_CoreSettings::Get();
	if (DevSettings && !DevSettings->IsValidThemeIndex(DevSettings->DefaultThemeIndex))
	{
		SetActiveThemeByIndex(DevSettings->DefaultThemeIndex);
		UE_LOG(LogModulusUI, Verbose, TEXT("UISubsystem: Loaded default theme from index %d"),
			DevSettings->DefaultThemeIndex);
	}
	
	UE_LOG(LogModulusUI, Log, TEXT("MCore_UISubsystem Initialized for LocalPlayer"));
}

void UMCore_UISubsystem::Deinitialize()
{
	UE_LOG(LogModulusUI, Log, TEXT("UISubsystem::Deinitialize - Cleaning up"));
	
	/** Clear delegate handle if still bound */
	if (PlayerControllerReadyHandle.IsValid())
	{
		if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
		{
			LocalPlayer->OnPlayerControllerChanged().Remove(PlayerControllerReadyHandle);
		}
		PlayerControllerReadyHandle.Reset();
	}
	
	/** Clear layer stack map (old references) */
	LayerStackMap.Empty();
	
	if (IsValid(CachedMenuHub))
	{
		CachedMenuHub->RemoveFromParent();
		CachedMenuHub = nullptr;
	}
	
	/** Clean up PrimaryGameLayout */
	if (IsValid(PrimaryGameLayout))
	{
		PrimaryGameLayout->RemoveFromParent();
		PrimaryGameLayout = nullptr;
	}
	
	RegisteredMenuScreens.Empty();
	CachedActiveTheme = nullptr;
	ActiveThemeIndex = INDEX_NONE;
	
	Super::Deinitialize();
}

//~ End of USubsystem Interface

//~ Start of Widget Class Loading

void UMCore_UISubsystem::LoadWidgetClasses()
{
	const UMCore_CoreSettings* DevSettings = UMCore_CoreSettings::Get();
	
	/** Load PrimaryGameLayout from DevSettings || use UISystem default */
	if (DevSettings && !DevSettings->PrimaryGameLayoutClass.IsNull())
	{
		PrimaryGameLayoutClass = DevSettings->PrimaryGameLayoutClass.LoadSynchronous();
	}
	
	if (!PrimaryGameLayoutClass)
	{
		PrimaryGameLayoutClass = UMCore_PrimaryGameLayout::StaticClass();
		UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: Using default PrimaryGameLayoutClass"));
	}
	
	/** Load MenuHubClass */
	if (DevSettings && !DevSettings->MenuHubClass.IsNull())
	{
		MenuHubClass = DevSettings->MenuHubClass.LoadSynchronous();
	}

	if (!MenuHubClass)
	{
		UE_LOG(LogModulusUI, Error, TEXT("UISubsystem: MenuHubClass is nullptr - check class exists"));
		MenuHubClass = UMCore_GameMenuHub::StaticClass();
	}
    
	/** Load ZOrder */
	if (DevSettings)
	{
		PrimaryGameLayoutZOrder = DevSettings->PrimaryGameLayoutZOrder;
	}
	
	UE_LOG(LogModulusUI, Verbose, 
		TEXT("UISubsystem: Widget classes loaded - MenuHub: %s"),
		MenuHubClass ? TEXT("OK") : TEXT("FAIL"));
}

//~ End of Widget Class Loading

//~ Begin Primary Game Layout

UMCore_PrimaryGameLayout* UMCore_UISubsystem::GetPrimaryGameLayout() const
{
	if (IsValid(PrimaryGameLayout)) { return PrimaryGameLayout; }
    
	UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem: No PrimaryGameLayout registered for this local player"));
	return nullptr;
}

void UMCore_UISubsystem::CreatePrimaryGameLayout()
{
	if (IsValid(PrimaryGameLayout)) { return; }
	
	if (!PrimaryGameLayoutClass)
	{
		UE_LOG(LogModulusUI, Error, TEXT("UISubsystem: PrimaryGameLayoutClass not set"));
		return;
	}
	
	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer) { return; }
	
	APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld());
	if (!PlayerController)
	{
		/** PlayerController not yet ready -- defer creation */
		UE_LOG(LogModulusUI, Verbose, TEXT("UISubsystem: Deferring layout creation until PlayerController ready"));
		
		if (ULocalPlayer* ThisPlayer = GetLocalPlayer())
		{
			PlayerControllerReadyHandle = ThisPlayer->OnPlayerControllerChanged().AddUObject(
				this, &UMCore_UISubsystem::OnPlayerControllerReady);
		}
		return;
	}
	
	/** Create Widget */
	PrimaryGameLayout = CreateWidget<UMCore_PrimaryGameLayout>(PlayerController, PrimaryGameLayoutClass);
	if (!IsValid(PrimaryGameLayout))
	{
		UE_LOG(LogModulusUI, Error, TEXT("UISubsystem: Failed to create PrimaryGameLayout widget"));
		return;
	}
	
	PrimaryGameLayout->AddToPlayerScreen(PrimaryGameLayoutZOrder);
	BuildLayerStackMap();
		
	UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: PrimaryGameLayout created (ZOrder: %d)"),
		PrimaryGameLayoutZOrder);
	
	/** Notify Subclasses and Listeners (BP Native Event + Multicast) */
	OnPrimaryGameLayoutCreated(PrimaryGameLayout);
	OnPrimaryGameLayoutReady.Broadcast(PrimaryGameLayout);
}

void UMCore_UISubsystem::OnPlayerControllerReady(APlayerController* PlayerController)
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		LocalPlayer->OnPlayerControllerChanged().Remove(PlayerControllerReadyHandle);
		PlayerControllerReadyHandle.Reset(); 
	}
	
	if (IsValid(PlayerController) && !IsValid(PrimaryGameLayout))
	{
		UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: PlayerController initialized, creating PrimaryGameLayout"));
		CreatePrimaryGameLayout();
	}
}

void UMCore_UISubsystem::OnPrimaryGameLayoutCreated_Implementation(UMCore_PrimaryGameLayout* Layout)
{
	/** 
	 * Empty by default
	 * Blueprint or C++ subclasses can override for custom setup
	 */
}

void UMCore_UISubsystem::BuildLayerStackMap()
{
	LayerStackMap.Empty();
	
	if (!IsValid(PrimaryGameLayout))
	{
		UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem: Cannot build layer map - no PrimaryGameLayout"));
		return;
	}
	
	using namespace MCore_UILayerTags;
	
	// Map tags to layer stacks
	if (PrimaryGameLayout->MCore_GameLayer)
	{
		LayerStackMap.Add(MCore_UI_Layer_Game, PrimaryGameLayout->MCore_GameLayer);
	}
	
	if (PrimaryGameLayout->MCore_GameMenuLayer)
	{
		LayerStackMap.Add(MCore_UI_Layer_GameMenu, PrimaryGameLayout->MCore_GameMenuLayer);
	}
	
	if (PrimaryGameLayout->MCore_MenuLayer)
	{
		LayerStackMap.Add(MCore_UI_Layer_Menu, PrimaryGameLayout->MCore_MenuLayer);
	}
	
	if (PrimaryGameLayout->MCore_ModalLayer)
	{
		LayerStackMap.Add(MCore_UI_Layer_Modal, PrimaryGameLayout->MCore_ModalLayer);
	}
	
	UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: LayerStackMap created with %d layers"), LayerStackMap.Num());
}

//~ End of Primary Game Layout

//~ Begin of Layer Stack Accessors

UCommonActivatableWidgetStack* UMCore_UISubsystem::GetLayerStack(FGameplayTag LayerTag) const
{
	if (!LayerTag.IsValid())
	{
		UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem::GetLayerStack: Invalid LayerTag"));
		return nullptr;
	}
	
	if (const TObjectPtr<UCommonActivatableWidgetStack>* ThisStack = LayerStackMap.Find(LayerTag))
	{
		return *ThisStack;
	}
	
	UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem::GetLayerStack: No stack found for tag '%s'"),
		*LayerTag.ToString());
	return nullptr;
}

UCommonActivatableWidget* UMCore_UISubsystem::PushWidgetToLayer(
	TSubclassOf<UCommonActivatableWidget> WidgetClass,
	FGameplayTag LayerTag)
{
	if (!WidgetClass)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem::PushWidgetToLayer: Invalid WidgetClass"));
		return nullptr;
	}
	
	UCommonActivatableWidgetStack* ThisStack = GetLayerStack(LayerTag);
	if (!ThisStack)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem::PushWidgetToLayer: No stack for tag '%s'"),
			*LayerTag.ToString());
		return nullptr;
	}
	
	/** AddWidget handles creation + activation */
	UCommonActivatableWidget* NewWidget = ThisStack->AddWidget<UCommonActivatableWidget>(WidgetClass);
	
	if (NewWidget)
	{
		UE_LOG(LogModulusUI, Verbose, TEXT("UISubsystem::PushWidgetToLayer: Pushed '%s' to layer '%s'"),
			*WidgetClass->GetName(), *LayerTag.ToString());
	}
	else
	{
		UE_LOG(LogModulusUI, Verbose, TEXT("UISubsystem::PushWidgetToLayer: Failed to push '%s' to layer '%s'"),
			*WidgetClass->GetName(), *LayerTag.ToString());
	}
	
	return NewWidget;
}

bool UMCore_UISubsystem::IsLayerActive(FGameplayTag LayerTag) const
{
	const UCommonActivatableWidgetStack* ThisStack = GetLayerStack(LayerTag);
	if (!ThisStack) { return false; }
	
	return ThisStack->GetActiveWidget() != nullptr;
}

//~ End of Layer Stack Accessors

//~ Start of Menu Hub

UMCore_GameMenuHub* UMCore_UISubsystem::GetOrCreateMenuHub()
{
	/** Return cached instance if valid */
	if (IsValid(CachedMenuHub)) { return CachedMenuHub; }
    
	if (!MenuHubClass)
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("UISubsystem: Cannot create MenuHub - class not loaded"));
		return nullptr;
	}
    
	if (!IsValid(PrimaryGameLayout))
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
	if (!IsValid(CachedMenuHub))
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
    
	return CachedMenuHub;
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
	if (IsValid(CachedMenuHub))
	{
		CachedMenuHub->RebuildTabBar();
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

		/** Rebuild MenuHub if it exists */
		if (IsValid(CachedMenuHub))
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
	if (IsValid(CachedMenuHub))
	{
		CachedMenuHub->RebuildTabBar();
	}
}

//~ End of Menu Hub

//~ Start of Theme System

const TArray<FMCore_ThemeEntry>& UMCore_UISubsystem::GetAvailableThemes() const
{
	const UMCore_CoreSettings* Settings = UMCore_CoreSettings::Get();
	if (Settings)
	{
		return Settings->AvailableThemes;
	}

	static TArray<FMCore_ThemeEntry> EmptyArray;
	return EmptyArray;
}

bool UMCore_UISubsystem::SetActiveThemeByIndex(int32 ThemeIndex)
{
	const UMCore_CoreSettings* Settings = UMCore_CoreSettings::Get();
	if (!Settings || !Settings->IsValidThemeIndex(ThemeIndex))
	{
		UE_LOG(LogModulusUI, Warning, TEXT("SetActiveThemeByIndex: Invalid theme index %d"), ThemeIndex);
		return false;
	}

	if (ThemeIndex == ActiveThemeIndex) { return true; }

	const FMCore_ThemeEntry& ThemeEntry = Settings->AvailableThemes[ThemeIndex];
	if (ThemeEntry.ThemeAsset.IsNull())
	{
		UE_LOG(LogModulusUI, Warning, TEXT("SetActiveThemeByIndex: Theme at index %d has no asset"), ThemeIndex);
		return false;
	}

	CachedActiveTheme = ThemeEntry.ThemeAsset.LoadSynchronous();
	ActiveThemeIndex = ThemeIndex;
	OnThemeChanged.Broadcast(CachedActiveTheme);

	UE_LOG(LogModulusUI, Log, TEXT("Theme changed to: %s (index %d)"),
		*ThemeEntry.DisplayName.ToString(), ThemeIndex);

	return true;
}

//~ End of Theme System