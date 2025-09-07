// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
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
    WidgetT* PushWidgetToLayer(FGameplayTag LayerTag,
        TSubclassOf<WidgetT> WidgetClass,
        const FString& RequestingPlugin = TEXT("Unknown"))
    {
        static_assert(TIsDerivedFrom<WidgetT, UCommonActivatableWidget>::IsDerived,
            "WidgetT must derive from UCommonActivatableWidget");

        UCommonActivatableWidgetStack* LayerStack = GetLayerStack(LayerTag);
        if (!LayerStack || !WidgetClass) { return nullptr; }

        if (WidgetT* NewWidget = LayerStack->AddWidget<WidgetT>(WidgetClass))
        {
            HandleWidgetPushedToLayer(LayerTag, NewWidget, RequestingPlugin);
            return NewWidget;
        }
        return nullptr;
    }

    // BP-Safe Widget Push (Sync)
    UFUNCTION(BlueprintCallable, Category = "UI|WidgetManagement")
    UCommonActivatableWidget* PushWidgetToLayer(FGameplayTag LayerTag, 
                                                TSubclassOf<UCommonActivatableWidget> WidgetClass,
                                                const FString& RequestingPlugin = TEXT("Unknown"));
    
    // Templated Async Widget Push
    template<typename WidgetT = UCommonActivatableWidget>
    void PushWidgetToLayerAsync(FGameplayTag LayerTag,
        TSoftClassPtr<WidgetT> WidgetClass,
        TFunction<void(WidgetT*)> OnCompleteCallback = nullptr,
        const FString& RequestingPlugin = TEXT("Unknown"),
        bool bSuspendInputUntilComplete = true)
    {
        static_assert(TIsDerivedFrom<WidgetT, UCommonActivatableWidget>::IsDerived,
        "WidgetT must derive from UCommonActivatableWidget");

        if (!WidgetClass.IsValid())
        {
            if (OnCompleteCallback) OnCompleteCallback(nullptr);
            return;
        }

        // (Fast Path) if Widget already loaded
        if (UClass* LoadedClass = WidgetClass.Get())
        {
            WidgetT* NewWidget = PushWidgetToLayer(LayerTag, LoadedClass, RequestingPlugin);
            if (OnCompleteCallback) OnCompleteCallback(NewWidget);
            return;
        }

        // Async load
        FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
        StreamableManager.RequestAsyncLoad(
            WidgetClass.ToSoftObjectPath(),
            FStreamableDelegate::CreateUObject(this, &UMCore_PrimaryGameLayout::HandleAsyncWidgetLoaded<WidgetT>,
                LayerTag, WidgetClass, OnCompleteCallback, RequestingPlugin, bSuspendInputUntilComplete)
                );
    }
    
    // BP-Safe Async Widget Push
    UFUNCTION(BlueprintCallable, Category = "UI|WidgetManagement")  
    void PushWidgetToLayerAsyncBP(FGameplayTag LayerTag,
                               TSoftClassPtr<UCommonActivatableWidget> WidgetClass,
                               const FString& RequestingPlugin = TEXT("Unknown"),
                               bool bSuspendInputUntilComplete = true);
    
    // Standard layer operations
    UFUNCTION(BlueprintCallable, Category = "UI|WidgetManagement")
    bool PopWidgetFromLayer(FGameplayTag LayerTag);

    UFUNCTION(BlueprintCallable, Category = "UI")
    UCommonActivatableWidgetStack* GetLayerStack(FGameplayTag LayerTag) const;

    UPROPERTY(BlueprintAssignable, Category = "UI|Events")
    FOnAsyncWidgetLoadedDelegate OnAsyncWidgetLoaded;
    
protected:
    virtual void NativeOnInitialized() override;

    template<typename WidgetT>
    void HandleAsyncWidgetLoaded(FGameplayTag LayerTag,
        TSoftClassPtr<WidgetT> WidgetClass,
        TFunction<void(WidgetT*)> OnCompleteCallback,
        const FString& RequestingPlugin,
        bool bSuspendInputUntilComplete)
    {
        if (UClass* LoadedClass = WidgetClass.Get())
        {
            WidgetT* NewWidget = PushWidgetToLayer<WidgetT>(
                LayerTag, LoadedClass, RequestingPlugin);
            if (OnCompleteCallback) OnCompleteCallback(NewWidget);
        }
        else
        {
            if (OnCompleteCallback) OnCompleteCallback(nullptr);
        }
    }

    void HandleAsyncWidgetLoadedBP(FGameplayTag LayerTag,
        TSoftClassPtr<UCommonActivatableWidget> WidgetClass,
        FString RequestingPlugin,
        bool bSuspendInputUntilComplete);
    
    
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

    UPROPERTY(Transient)
    TMap<FGameplayTag, FString> LastRequestingPluginPerLayer;
    
    void InitializeLayerMap();
    
    void HandleWidgetPushedToLayer(FGameplayTag LayerTag,
        UCommonActivatableWidget* PushedWidget,
        const FString& RequestingPlugin);
};
