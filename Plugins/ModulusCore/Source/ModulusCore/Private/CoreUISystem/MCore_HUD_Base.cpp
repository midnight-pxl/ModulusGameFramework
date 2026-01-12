// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreUISystem/MCore_HUD_Base.h"

#include "CoreData/CoreLogging/LogModulusUI.h"
#include "CoreUISystem/CoreWidgets/MCore_PrimaryGameLayout.h"
#include "CoreUISystem/MCore_UISubsystem.h"

AMCore_HUD_Base::AMCore_HUD_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryGameLayoutClass = UMCore_PrimaryGameLayout::StaticClass();
}

void AMCore_HUD_Base::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetNetMode() == NM_DedicatedServer)
	{
		UE_LOG(LogModulusUI, Log, TEXT("BeginPlay: Skipping UI creation on dedicated server"));
		return;
	}
	
	APlayerController* OwningPlayer = GetOwningPlayerController();
	if (!OwningPlayer)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("BeginPlay: No valid owning PlayerController, cannot create PrimaryGameLayout"));
		return;
	}
	
	if (!OwningPlayer->IsLocalController())
	{
		UE_LOG(LogModulusUI, Log, TEXT("BeginPlay: Skipping UI for non-local Player"));
		return;
	}
	
	/** Check if Layout already exists (seamless travel == widget survived w/ PlayerController) */
	if (UMCore_UISubsystem* UISubsystem = GetUISubsystem())
	{
		UMCore_PrimaryGameLayout* ExistingLayout = UISubsystem->GetPrimaryGameLayout();
		if (IsValid(ExistingLayout))
		{
			/** Reuse the already existing layout */
			PrimaryGameLayout = ExistingLayout;
			UE_LOG(LogModulusUI, Log, TEXT("BeginPlay: Reusing existing PrimaryGameLayout"));
			
			/** Fire notify in case any updates need to occur */
			OnPrimaryGameLayoutCreated(PrimaryGameLayout);
			return;
		}
	}
	
	/** No existing PrimaryGameLayout so create new */
	CreatePrimaryGameLayout();
}

void AMCore_HUD_Base::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	/**
	 * Clear local references only - don't destroy widget
	 * Widget will survive if PlayerController does
	 */
	PrimaryGameLayout = nullptr;
	CachedUISubsystem = nullptr;
	
	UE_LOG(LogModulusUI, Log, TEXT("EndPlay: Reason == %d - Cleared references, widget preserved for seamless travel"),
		static_cast<int32>(EndPlayReason));
	
	Super::EndPlay(EndPlayReason);
}

void AMCore_HUD_Base::CreatePrimaryGameLayout()
{
	if (!PrimaryGameLayoutClass)
	{
		UE_LOG(LogModulusUI, Error, TEXT("CreatePrimaryGameLayout: Cannot create, PrimaryGameLayoutClass unset"));
		return;
	}
	
	APlayerController* OwningPlayer = GetOwningPlayerController();
	if (!OwningPlayer)
	{
		UE_LOG(LogModulusUI, Error, TEXT("CreatePrimaryGameLayout: PlayerController reference currently invalid"));
		return;
	}
	
	PrimaryGameLayout = CreateWidget<UMCore_PrimaryGameLayout>(OwningPlayer, PrimaryGameLayoutClass);
	if (!PrimaryGameLayout)
	{
		UE_LOG(LogModulusUI, Error, TEXT("CreatePrimaryGameLayout: Failed to create PrimaryGameLayout widget"));
		return;
	}
	
	/** Add to Player's viewport at specific Z-Order */
	PrimaryGameLayout->AddToPlayerScreen(PrimaryGameLayoutZOrder);
	
	UE_LOG(LogModulusUI, Log, TEXT("Created PrimaryGameLayout widget"));
	/** Register w/ UISubsystem */
	RegisterLayoutWithSubsystem();
	
	/** Notify BP and/or derived classes */
	OnPrimaryGameLayoutCreated(PrimaryGameLayout);
}

void AMCore_HUD_Base::RegisterLayoutWithSubsystem()
{
	if (!PrimaryGameLayout)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("RegisterLayoutWithSubsystem: UISubsystem not available, registration deferred"));
		/** PrimaryGameLayout will self-register in NativeOnInitialized */
		return;
	}
	
	/** Register w/ UISubsystem */
	const bool bIsRegistered = CachedUISubsystem->RegisterPrimaryGameLayout(PrimaryGameLayout);
	if (!bIsRegistered)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("RegisterLayoutWithSubsystem: Failed to register PrimaryGameLayout, may already exist"));
		return;
	}
	
	UE_LOG(LogModulusUI, Log, TEXT("RegisterLayoutWithSubsystem: Registered PrimaryGameLayout with UISubsystem"))
}

void AMCore_HUD_Base::CleanupPrimaryGameLayout()
{
	/**
	 * Explicit cleanup - call manually when you want to destroy the layout
	 * Use cases: returning to main menu, full game reset, testing
	 * 
	 * Unregister from UISubsystem */
	if (UMCore_UISubsystem* UISubsystem = GetUISubsystem()) { UISubsystem->UnregisterPrimaryGameLayout(); }
	
	if (IsValid(PrimaryGameLayoutClass))
	{
		UE_LOG(LogModulusUI, Log, TEXT("CleanupPrimaryGameLayout: Cleaning up PrimaryGameLayout '%s'"),
			*GetNameSafe(PrimaryGameLayout));
		
		/** Remove from Player Screen (viewport) */
		PrimaryGameLayout->RemoveFromParent();
		
		/** Clear  */
		PrimaryGameLayout = nullptr;
	}
}

UMCore_UISubsystem* AMCore_HUD_Base::GetUISubsystem()
{
	if (IsValid(CachedUISubsystem)) { return CachedUISubsystem; }
	
	/** Early return checks */
	APlayerController* OwningPlayer = GetOwningPlayerController();
	if (!OwningPlayer) { return nullptr; }
	
	ULocalPlayer* LocalPlayer = OwningPlayer->GetLocalPlayer();
	if (!LocalPlayer) { return nullptr; }
	
	// Get UISubsystem from local player
	CachedUISubsystem = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
	return CachedUISubsystem;
}

void AMCore_HUD_Base::OnPrimaryGameLayoutCreated_Implementation(UMCore_PrimaryGameLayout* Layout)
{
	/**
	 * Override in Blueprint or derived C++ class for custom setup
	 * Examples:
	 * - Bind to layout events 
	 * - Spawn additional HUD elements 
	 * - Configure layer stack defaults
	 */
}
