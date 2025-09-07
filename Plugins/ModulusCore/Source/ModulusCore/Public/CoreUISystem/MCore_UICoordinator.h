// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MCore_ActivatableWidget.h"
#include "MCore_UICoordinator.generated.h"

class UCommonActivatableWidgetStack;

/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_UICoordinator : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    // Core coordination: Priority-based widget pushing
    UFUNCTION(BlueprintCallable, Category = "UI|LayerManagement")
    bool TryPushWidgetToLayer(FGameplayTag LayerTag, 
                             TSubclassOf<UCommonActivatableWidget> WidgetClass,
                             EMCore_UIPriority DesiredPriority,
                             const FString& RequestingPlugin);

    // Check if a priority can interrupt current layer owner
    UFUNCTION(BlueprintCallable, Category = "UI|LayerManagement")
    bool CanPushWithPriority(FGameplayTag LayerTag, EMCore_UIPriority DesiredPriority) const;

    // Get primary layout reference (set by layout on creation)
    UFUNCTION(BlueprintCallable, Category = "UI|LayerManagement")
    class UMCore_PrimaryGameLayout* GetPrimaryGameLayout() const;

    UFUNCTION(BlueprintCallable, Category = "UI|LayerManagement")
    void SetPrimaryGameLayout(class UMCore_PrimaryGameLayout* Layout);

    // Extension point: Custom layer registration (for edge cases only)
    UFUNCTION(BlueprintCallable, Category = "UI|LayerManagement")
    void RegisterCustomLayer(FGameplayTag LayerTag, UCommonActivatableWidgetStack* LayerStack);

    // Widget lifecycle tracking (called by MCore_ActivatableWidget)
    void OnWidgetActivated(UMCore_ActivatableWidget* Widget);
    void OnWidgetDeactivated(UMCore_ActivatableWidget* Widget);

protected:
    // Primary layout reference
    UPROPERTY()
    TObjectPtr<class UMCore_PrimaryGameLayout> PrimaryGameLayout;

    // Priority tracking per layer
    UPROPERTY()
    TMap<FGameplayTag, EMCore_UIPriority> LayerPriorities;

    // Plugin tracking per layer (for debugging)
    UPROPERTY()
    TMap<FGameplayTag, FString> LayerOwners;

    /**
     * Extension: Custom layers (empty until needed)
     * UPROPERTY()
     * TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetStack>> CustomLayers;
     */

private:
    // Find the appropriate widget stack for a layer
    UCommonActivatableWidgetStack* GetLayerStack(FGameplayTag LayerTag) const;
};
