// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MCore_UILayerSubsystem.generated.h"

class UCommonActivatableWidget;

/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_UILayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Standard layer access - follows Lyra's proven pattern
	UFUNCTION(BlueprintPure, Category = "UI|Stack")
	UCommonActivatableWidgetStack* GetGameLayer() const;
    
	UFUNCTION(BlueprintPure, Category = "UI|Stack") 
	UCommonActivatableWidgetStack* GetGameMenuLayer() const;
    
	UFUNCTION(BlueprintPure, Category = "UI|Stack")
	UCommonActivatableWidgetStack* GetMenuLayer() const;
    
	UFUNCTION(BlueprintPure, Category = "UI|Stack")
	UCommonActivatableWidgetStack* GetModalLayer() const;

	// Framework-specific Blueprint helpers (not UE wrappers)
	UFUNCTION(BlueprintCallable, Category = "UI|Stack")
	void PushWidgetToGameLayer(TSubclassOf<UCommonActivatableWidget> WidgetClass);
    
	UFUNCTION(BlueprintCallable, Category = "UI|Stack")
	void PushWidgetToModalLayer(TSubclassOf<UCommonActivatableWidget> WidgetClass);

private:
	// Uses UE's CommonUI stacks, just organizes them
	UPROPERTY()
	TObjectPtr<UCommonActivatableWidgetStack> GameLayerStack;
    
	UPROPERTY()
	TObjectPtr<UCommonActivatableWidgetStack> GameMenuLayerStack;
};
