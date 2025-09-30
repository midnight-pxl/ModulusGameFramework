// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "MCore_UISubsystem.generated.h"

class UMCore_PrimaryGameLayout;

/**
 * Per-player UI subsystem providing standardized access to PrimaryGameLayout
 * Scoped to local players - zero server overhead, split-screen compatible
 */
UCLASS()
class MODULUSCORE_API UMCore_UISubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "UI Layout")
	UMCore_PrimaryGameLayout* GetPrimaryGameLayout() const;

	void RegisterPrimaryGameLayout(UMCore_PrimaryGameLayout* InLayout);

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UMCore_PrimaryGameLayout> CachedPrimaryGameLayout;
};
