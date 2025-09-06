// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreNetworkingSystem/MCore_NetworkingComponent.h"
#include "MCore_TaggableComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MODULUSCORE_API UMCore_TaggableComponent : public UMCore_NetworkingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMCore_TaggableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
