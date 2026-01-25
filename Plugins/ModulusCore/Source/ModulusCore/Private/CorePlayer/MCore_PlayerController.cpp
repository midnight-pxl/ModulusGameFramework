// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CorePlayer/MCore_PlayerController.h"
#include "CoreUI/Widgets/MCore_PrimaryGameLayout.h"
#include "CoreData/Tags/MCore_UISettingsTags.h"
#include "CoreData\Logging\LogModulusUI.h"
#include "CoreUI/Widgets/Primitives/MCore_ActivatableBase.h"
#include "Net/UnrealNetwork.h"

AMCore_PlayerController::AMCore_PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMCore_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController() && ShouldCreatePGLayout())
	{
		CreatePGLayout();
	}
}

/**
 * UI Layer Management
 */

bool AMCore_PlayerController::ShouldCreatePGLayout_Implementation() const
{
	return !IsRunningDedicatedServer() && PrimaryGameLayoutClass != nullptr;
}

void AMCore_PlayerController::OnPGLayoutReady_Implementation()
{
	/** Override in child classes/blueprints */
}

void AMCore_PlayerController::CreatePGLayout()
{
	if (PrimaryGameLayout) { return; }
	
	PrimaryGameLayout = CreateWidget<UMCore_PrimaryGameLayout>(this, PrimaryGameLayoutClass);
	if (PrimaryGameLayout)
	{
		PrimaryGameLayout->AddToViewport(0);
		OnPGLayoutReady();
	}
}

void AMCore_PlayerController::SetMenuInputMode()
{
}

UMCore_ActivatableBase* AMCore_PlayerController::PushWidgetToLayer(TSubclassOf<UMCore_ActivatableBase> WidgetClass,
                                                                   FGameplayTag LayerTag)
{
	if (!PrimaryGameLayout || !WidgetClass) { return nullptr; }
	
	// STUB
	return nullptr;
}

void AMCore_PlayerController::RemoveWidget(UMCore_ActivatableBase* Widget)
{
}

void AMCore_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AMCore_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AMCore_PlayerController::ToggleInGameMenu()
{
}















