// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "MCore_PrimaryGameLayout.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAsyncWidgetLoadedDelegate, UCommonActivatableWidget*, Widget, bool, bSuccess);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MODULUSCORE_API UMCore_PrimaryGameLayout : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
    UMCore_PrimaryGameLayout(const FObjectInitializer& ObjectInitializer);

    // Templated Widget Push (Sync)
    template<typename WidgetT = UCommonActivatableWidget>
    WidgetT* PushWidgetToLayer(const FGameplayTag& LayerTag,
        TSubclassOf<WidgetT> WidgetClass)
    {
        static_assert(TIsDerivedFrom<WidgetT, UCommonActivatableWidget>::IsDerived,
            "WidgetT must derive from UCommonActivatableWidget");

        UCommonActivatableWidgetStack* LayerStack = GetLayerStack(LayerTag);
        if (!LayerStack || !WidgetClass) { return nullptr; }

        return LayerStack->AddWidget<WidgetT>(WidgetClass);
    }

    // BP-Safe Widget Push (Sync)
    UFUNCTION(BlueprintCallable, Category = "UI|WidgetManagement")
    UCommonActivatableWidget* PushWidgetToLayer(const FGameplayTag& LayerTag,
        TSubclassOf<UCommonActivatableWidget> WidgetClass);
    
    // BP-Safe Async Widget Push
    UFUNCTION(BlueprintCallable, Category = "UI|WidgetManagement")  
    void PushWidgetToLayerAsync(const FGameplayTag& LayerTag,
        TSoftClassPtr<UCommonActivatableWidget> WidgetClass);
    
    // Standard layer operations
    UFUNCTION(BlueprintCallable, Category = "UI|WidgetManagement")
    bool PopWidgetFromLayer(const FGameplayTag& LayerTag);

    UFUNCTION(BlueprintCallable, Category = "UI")
    UCommonActivatableWidgetStack* GetLayerStack(const FGameplayTag& LayerTag) const;

    UPROPERTY(BlueprintAssignable, Category = "UI|Events")
    FOnAsyncWidgetLoadedDelegate OnAsyncWidgetLoaded;
    
protected:
    virtual void NativeOnInitialized() override;

    // Widget Stack References
    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> GameLayer;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> GameMenuLayer;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> MenuLayer;

    UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "UI|Layers")
    TObjectPtr<UCommonActivatableWidgetStack> ModalLayer;

private:
    // Layer registry for fast lookup
    UPROPERTY()
    TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetStack>> LayerMap;
    
    void InitializeLayerMap();
    void HandleAsyncWidgetLoaded(const FGameplayTag& LayerTag,
        TSoftClassPtr<UCommonActivatableWidget> WidgetClass);
};