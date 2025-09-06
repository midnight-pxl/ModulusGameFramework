// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreTaggingSystem/MCore_TaggableComponent.h"


// Sets default values for this component's properties
UMCore_TaggableComponent::UMCore_TaggableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMCore_TaggableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMCore_TaggableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

