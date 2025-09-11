// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreEventSystem/MCore_EventListenerComponent.h"


// Sets default values for this component's properties
UMCore_EventListenerComponent::UMCore_EventListenerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UMCore_EventListenerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
