// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUI/MCore_UISubsystem.h"

#include "GameplayTagContainer.h"
#include "Engine/LocalPlayer.h"

#include "CoreData/Logging/LogModulusUI.h"
#include "CoreData/DevSettings/MCore_CoreSettings.h"
#include "CoreUI/Settings/MCore_PlayerSettingsSave.h"
#include "CoreData/Tags/MCore_UILayerTags.h"
#include "CoreUI/Widgets/MCore_GameMenuHub.h"
#include "CoreUI/Widgets/MCore_PrimaryGameLayout.h"
#include "CoreData/Assets/UI/Themes/MCore_PDA_UITheme_Base.h"
#include "CoreData/Libraries/MCore_GameSettingsLibrary.h"
#include "CoreData/Tags/MCore_SettingsTags.h"

// ============================================================================
// INITIALIZATION
// ============================================================================

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
	
	// Load theming for widget creation
	const UMCore_CoreSettings* DevSettings = UMCore_CoreSettings::Get();
	if (DevSettings && DevSettings->IsValidThemeIndex(DevSettings->DefaultThemeIndex))
	{
		SetActiveThemeByIndex(DevSettings->DefaultThemeIndex);
		UE_LOG(LogModulusUI, Verbose, TEXT("UISubsystem: Loaded default theme from index %d"),
			DevSettings->DefaultThemeIndex);
	}

	// Register default menu tabs from CoreSettings
	if (DevSettings)
	{
		for (const FMCore_MenuTab& Tab : DevSettings->DefaultMenuTabs)
		{
			if (Tab.TabID.IsValid() && Tab.ScreenWidgetClass)
			{
				RegisterMenuScreen(Tab.TabID, Tab.ScreenWidgetClass, Tab.Priority, Tab.TabIcon);
			}
			else
			{
				UE_LOG(LogModulusUI, Warning,
					TEXT("DefaultMenuTabs: skipping entry with invalid TabID or null ScreenWidgetClass"));
			}
		}
	}

	UE_LOG(LogModulusUI, Log, TEXT("MCore_UISubsystem Initialized for LocalPlayer"));
}

void UMCore_UISubsystem::Deinitialize()
{
	UE_LOG(LogModulusUI, Log, TEXT("UISubsystem::Deinitialize - Cleaning up"));
	
	// Clear delegate handle if still bound
	if (PlayerControllerReadyHandle.IsValid())
	{
		if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
		{
			LocalPlayer->OnPlayerControllerChanged().Remove(PlayerControllerReadyHandle);
		}
		PlayerControllerReadyHandle.Reset();
	}
	
	// Save and release player settings
	if (CachedPlayerSettings)
	{
		CachedPlayerSettings->SaveSettings();
		CachedPlayerSettings = nullptr;
	}
	
	// Clear layer stack map and tracked widgets
	LayerStackMap.Empty();
	TrackedWidgets.Empty();

	// Clean up PrimaryGameLayout
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

// ============================================================================
// CONFIGURATION
// ============================================================================

void UMCore_UISubsystem::LoadWidgetClasses()
{
	const UMCore_CoreSettings* DevSettings = UMCore_CoreSettings::Get();
	
	// Load PrimaryGameLayout from DevSettings or use default
	if (DevSettings && !DevSettings->PrimaryGameLayoutClass.IsNull())
	{
		PrimaryGameLayoutClass = DevSettings->PrimaryGameLayoutClass.LoadSynchronous();
	}
	
	if (!PrimaryGameLayoutClass)
	{
		PrimaryGameLayoutClass = UMCore_PrimaryGameLayout::StaticClass();
		UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: Using default PrimaryGameLayoutClass"));
	}
	
	// Load MenuHubClass
	if (DevSettings && !DevSettings->MenuHubClass.IsNull())
	{
		MenuHubClass = DevSettings->MenuHubClass.LoadSynchronous();
	}

	if (!MenuHubClass)
	{
		UE_LOG(LogModulusUI, Error, TEXT("UISubsystem: MenuHubClass is nullptr - check class exists"));
		MenuHubClass = UMCore_GameMenuHub::StaticClass();
	}
    
	// Load ZOrder
	if (DevSettings)
	{
		PrimaryGameLayoutZOrder = DevSettings->PrimaryGameLayoutZOrder;
	}
	
	UE_LOG(LogModulusUI, Verbose, 
		TEXT("UISubsystem: Widget classes loaded - MenuHub: %s"),
		MenuHubClass ? TEXT("OK") : TEXT("FAIL"));
}

// ============================================================================
// WIDGET LIFECYCLE
// ============================================================================

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
		// PlayerController not yet ready, defer creation
		UE_LOG(LogModulusUI, Verbose, TEXT("UISubsystem: Deferring layout creation until PlayerController ready"));
		
		if (ULocalPlayer* ThisPlayer = GetLocalPlayer())
		{
			PlayerControllerReadyHandle = ThisPlayer->OnPlayerControllerChanged().AddUObject(
				this, &UMCore_UISubsystem::OnPlayerControllerReady);
		}
		return;
	}
	
	// Create widget
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
	
	// Notify subclasses and listeners
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
	// Default empty; override in Blueprint or C++ subclasses
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

// ============================================================================
// PLAYER SETTINGS
// ============================================================================

UMCore_PlayerSettingsSave* UMCore_UISubsystem::GetPlayerSettings()
{
	if (!CachedPlayerSettings)
	{
		CachedPlayerSettings = UMCore_PlayerSettingsSave::LoadPlayerSettings();

		UE_LOG(LogModulusUI, Log, TEXT("UISubsystem: Player settings loaded"));
	}

	return CachedPlayerSettings;
}

// ============================================================================
// LAYER STACKS
// ============================================================================

UCommonActivatableWidgetStack* UMCore_UISubsystem::GetLayerStack(FGameplayTag LayerTag) const
{
	if (!LayerTag.IsValid())
	{
		UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem::GetLayerStack: Invalid LayerTag"));
		return nullptr;
	}
	
	if (const TObjectPtr<UCommonActivatableWidgetStack> ThisStack = LayerStackMap.FindRef(LayerTag))
	{
		UE_LOG(LogModulusUI, Verbose, TEXT("GetLayerStack: Tag='%s', Stack=%s"),
			*LayerTag.ToString(), ThisStack ? *ThisStack->GetName() : TEXT("NULL"));
		
		return ThisStack;
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
	
	// AddWidget handles creation; activation is deferred until the stack's animated transition completes
	UCommonActivatableWidget* NewWidget = ThisStack->AddWidget<UCommonActivatableWidget>(WidgetClass);

	if (NewWidget)
	{
		TrackedWidgets.FindOrAdd(LayerTag).Add(NewWidget);
		OnWidgetPushed.Broadcast(NewWidget, LayerTag);

		UE_LOG(LogModulusUI, Log, TEXT("PushWidgetToLayer: Pushed '%s' to layer '%s'"),
			*WidgetClass->GetName(), *LayerTag.ToString());
	}
	else
	{
		UE_LOG(LogModulusUI, Warning, TEXT("PushWidgetToLayer: Failed to add '%s' to layer '%s'"),
			*WidgetClass->GetName(), *LayerTag.ToString());
	}
	
	return NewWidget;
}

UCommonActivatableWidget* UMCore_UISubsystem::OpenScreen(
	TSubclassOf<UCommonActivatableWidget> ScreenClass,
	FGameplayTag LayerTag,
	bool bAllowDuplicates)
{
	if (!ScreenClass || !HasPrimaryGameLayout()) { return nullptr; }

	if (!bAllowDuplicates)
	{
		CompactTrackedWidgets(LayerTag);

		TArray<TWeakObjectPtr<UCommonActivatableWidget>>* Widgets = TrackedWidgets.Find(LayerTag);

		if (Widgets)
		{
			for (int32 i = Widgets->Num() - 1; i >= 0; --i)
			{
				TWeakObjectPtr<UCommonActivatableWidget>& Weak = (*Widgets)[i];

				if (Weak->GetClass() == ScreenClass)
				{
					if (Weak->IsActivated())
					{
						return Weak.Get();
					}
					// Stale entry: tracked but no longer activated
					UntrackWidget(Weak.Get(), LayerTag);
				}
			}
		}
	}
	
	return PushWidgetToLayer(ScreenClass, LayerTag);
}

void UMCore_UISubsystem::CloseScreen(UCommonActivatableWidget* Screen)
{
	if (!Screen) { return; }

	bool bWasTracked = false;
	for (auto& Pair : TrackedWidgets)
	{
		const bool bFound = Pair.Value.ContainsByPredicate([Screen](const TWeakObjectPtr<UCommonActivatableWidget>& Ptr)
		{
			return Ptr.Get() == Screen;
		});

		if (bFound)
		{
			UntrackWidget(Screen, Pair.Key);
			bWasTracked = true;
			break;
		}
	}

	if (!bWasTracked)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("CloseScreen: Widget '%s' was not tracked"),
			*Screen->GetName());
	}

	Screen->DeactivateWidget();
}

bool UMCore_UISubsystem::PopLayer(FGameplayTag LayerTag)
{
	UCommonActivatableWidgetStack* Stack = GetLayerStack(LayerTag);
	if (!Stack) { return false; }

	UCommonActivatableWidget* TopWidget = Stack->GetActiveWidget();
	if (!TopWidget) { return false; }

	CloseScreen(TopWidget);
	return true;
}

bool UMCore_UISubsystem::RemoveWidgetFromLayer(UCommonActivatableWidget* Widget, FGameplayTag LayerTag)
{
	if (!Widget) { return false; }

	const TArray<TWeakObjectPtr<UCommonActivatableWidget>>* Widgets = TrackedWidgets.Find(LayerTag);
	if (!Widgets) { return false; }

	const bool bFound = Widgets->ContainsByPredicate([Widget](const TWeakObjectPtr<UCommonActivatableWidget>& Ptr)
	{
		return Ptr.Get() == Widget;
	});

	if (!bFound) { return false; }

	CloseScreen(Widget);
	return true;
}

UCommonActivatableWidget* UMCore_UISubsystem::GetActiveWidgetInLayer(FGameplayTag LayerTag) const
{
	const UCommonActivatableWidgetStack* Stack = GetLayerStack(LayerTag);
	return Stack ? Stack->GetActiveWidget() : nullptr;
}

int32 UMCore_UISubsystem::GetWidgetCountInLayer(FGameplayTag LayerTag) const
{
	const TArray<TWeakObjectPtr<UCommonActivatableWidget>>* Widgets = TrackedWidgets.Find(LayerTag);
	if (!Widgets) { return 0; }

	int32 Count = 0;
	for (const TWeakObjectPtr<UCommonActivatableWidget>& Weak : *Widgets)
	{
		if (Weak.IsValid()) { Count++; }
	}
	return Count;
}

bool UMCore_UISubsystem::IsLayerActive(FGameplayTag LayerTag) const
{
	return GetActiveWidgetInLayer(LayerTag) != nullptr;
}

// ============================================================================
// WIDGET TRACKING
// ============================================================================

void UMCore_UISubsystem::CompactTrackedWidgets(FGameplayTag LayerTag)
{
	TArray<TWeakObjectPtr<UCommonActivatableWidget>>* Widgets = TrackedWidgets.Find(LayerTag);
	if (!Widgets) { return; }

	Widgets->RemoveAllSwap([](const TWeakObjectPtr<UCommonActivatableWidget>& Ptr)
	{
		return !Ptr.IsValid();
	});
}

void UMCore_UISubsystem::UntrackWidget(UCommonActivatableWidget* Widget, FGameplayTag LayerTag)
{
	TArray<TWeakObjectPtr<UCommonActivatableWidget>>* Widgets = TrackedWidgets.Find(LayerTag);
	if (!Widgets) { return; }

	const int32 Removed = Widgets->RemoveAllSwap([Widget](const TWeakObjectPtr<UCommonActivatableWidget>& Ptr)
	{
		return Ptr.Get() == Widget;
	});

	if (Removed > 0)
	{
		OnWidgetRemoved.Broadcast(Widget, LayerTag);
	}
}

void UMCore_UISubsystem::NotifyWidgetDestroyed(UCommonActivatableWidget* Widget)
{
	if (!Widget) { return; }

	for (auto& Pair : TrackedWidgets)
	{
		const int32 Removed = Pair.Value.RemoveAllSwap([Widget](const TWeakObjectPtr<UCommonActivatableWidget>& Ptr)
		{
			return Ptr.Get() == Widget;
		});

		if (Removed > 0)
		{
			OnWidgetRemoved.Broadcast(Widget, Pair.Key);
		}
	}
}

// ============================================================================
// MENU HUB
// ============================================================================

UMCore_GameMenuHub* UMCore_UISubsystem::OpenMenuHub()
{
	if (!MenuHubClass)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("UISubsystem::OpenMenuHub: MenuHubClass not loaded"));
		return nullptr;
	}

	UCommonActivatableWidget* Screen = OpenScreen(MenuHubClass, MCore_UILayerTags::MCore_UI_Layer_GameMenu);
	UMCore_GameMenuHub* Hub = Cast<UMCore_GameMenuHub>(Screen);

	if (Hub)
	{
		Hub->RebuildTabBar();
	}

	return Hub;
}

void UMCore_UISubsystem::CloseMenuHub()
{
	if (UMCore_GameMenuHub* Hub = FindTrackedMenuHub())
	{
		CloseScreen(Hub);
	}
}

UMCore_GameMenuHub* UMCore_UISubsystem::FindTrackedMenuHub() const
{
	const TArray<TWeakObjectPtr<UCommonActivatableWidget>>* Widgets =
		TrackedWidgets.Find(MCore_UILayerTags::MCore_UI_Layer_GameMenu);
	if (!Widgets) { return nullptr; }

	for (const TWeakObjectPtr<UCommonActivatableWidget>& Weak : *Widgets)
	{
		if (UMCore_GameMenuHub* Hub = Cast<UMCore_GameMenuHub>(Weak.Get()))
		{
			return Hub;
		}
	}
	return nullptr;
}

/**
 * @param TabID              Unique tag identifying this tab. Duplicates are rejected with a warning.
 * @param ScreenWidgetClass  Widget class to instantiate as the tab's page content. Must be non-null.
 * @param Priority           Sort order (lower values appear first). Default: 100.
 */
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
	
	// Check for duplicate registration
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
    
	// Insert sorted by priority (lower = earlier in tab list)
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

	// If MenuHub is active, rebuild tab bar
	if (UMCore_GameMenuHub* Hub = FindTrackedMenuHub())
	{
		Hub->RebuildTabBar();
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

		if (UMCore_GameMenuHub* Hub = FindTrackedMenuHub())
		{
			Hub->RebuildTabBar();
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
	if (UMCore_GameMenuHub* Hub = FindTrackedMenuHub())
	{
		Hub->RebuildTabBar();
	}
}

// ============================================================================
// THEME
// ============================================================================

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

int32 UMCore_UISubsystem::GetActiveTextSizeIndex() const
{
	const ULocalPlayer* LP = GetLocalPlayer();
	if (!LP)
	{
		return 0;
	}

	const int32 RawIndex = UMCore_GameSettingsLibrary::GetSettingIntByTag(LP, MCore_SettingsTags::MCore_Settings_Accessibility_UITextSize);
	return FMath::Max(RawIndex, 0);
}

