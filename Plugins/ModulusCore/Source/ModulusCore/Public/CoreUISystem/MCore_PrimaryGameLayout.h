// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "Engine/StreamableManager.h"
#include "MCore_PrimaryGameLayout.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MODULUSCORE_API UMCore_PrimaryGameLayout : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
    UMCore_PrimaryGameLayout(const FObjectInitializer& ObjectInitializer);

    // Layer management API - integrates with priority system
    UFUNCTION(BlueprintCallable, Category = "Modulus UI")
    UCommonActivatableWidget* PushWidgetToLayer(FGameplayTag LayerTag, 
                                                TSubclassOf<UCommonActivatableWidget> WidgetClass,
                                                const FString& RequestingPlugin = TEXT("Unknown"));

    // Async loading with priority coordination
    UFUNCTION(BlueprintCallable, Category = "Modulus UI")  
    void PushWidgetToLayerAsync(FGameplayTag LayerTag,
                               TSoftClassPtr<UCommonActivatableWidget> WidgetClass,
                               const FString& RequestingPlugin = TEXT("Unknown"),
                               bool bSuspendInputUntilComplete = true);

    // Standard layer operations
    UFUNCTION(BlueprintCallable, Category = "Modulus UI")
    bool PopWidgetFromLayer(FGameplayTag LayerTag);

    UFUNCTION(BlueprintCallable, Category = "Modulus UI")
    UCommonActivatableWidgetStack* GetLayerStack(FGameplayTag LayerTag) const;

protected:
    virtual void NativeOnInitialized() override;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> GameLayer;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> GameMenuLayer;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> MenuLayer;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> ModalLayer;

private:
    // Layer registry for fast lookup
    UPROPERTY()
    TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetStack>> LayerMap;

    // Async loading management
    TSharedPtr<FStreamableHandle> ActiveStreamableHandle;

    // Initialize standard layer mapping
    void InitializeLayerMap();

    // Async loading callback
    void OnWidgetClassLoaded(TSoftClassPtr<UCommonActivatableWidget> WidgetClass, 
                            FGameplayTag LayerTag, 
                            FString RequestingPlugin,
                            bool bSuspendInputUntilComplete);
};
