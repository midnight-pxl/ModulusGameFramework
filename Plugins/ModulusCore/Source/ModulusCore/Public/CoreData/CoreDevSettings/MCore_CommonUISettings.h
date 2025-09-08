// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CoreUISystem/CoreGameSettings/MCore_GameSettingType.h"
#include "Engine/DeveloperSettings.h"
#include "MCore_CommonUISettings.generated.h"

class UMCore_UIThemeDataAsset_Base;

/**
 * 
 */
UCLASS(Config=ModulusGameFramework, DefaultConfig, meta=(DisplayName="Modulus UI Settings"))
class MODULUSCORE_API UMCore_CommonUISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UMCore_CommonUISettings();

	/**
	 * CommonUI Widget Stack Layer Tags
	 */
	UPROPERTY(config, EditAnywhere, Category = "Layer Tags",
		meta=(ExposeFunctionCategories="UI.Layer"))
	FGameplayTag GameLayerTag;

	UPROPERTY(config, EditAnywhere, Category = "Layer Tags",
	meta=(ExposeFunctionCategories="UI.Layer"))
	FGameplayTag GameMenuLayerTag;

	UPROPERTY(config, EditAnywhere, Category = "Layer Tags",
	meta=(ExposeFunctionCategories="UI.Layer"))
	FGameplayTag MenuLayerTag;

	UPROPERTY(config, EditAnywhere, Category = "Layer Tags",
	meta=(ExposeFunctionCategories="UI.Layer"))
	FGameplayTag ModalLayerTag;
	
	// Performance settings
	UPROPERTY(config, EditAnywhere, Category = "Performance")
	bool bPreloadCommonWidgets{true};

	UPROPERTY(config, EditAnywhere, Category = "Performance")
	float AsyncLoadTimeoutSeconds{3.0f};

	// Performance settings
	UPROPERTY(config, EditAnywhere, Category = "Performance")
	bool bEnableWidgetPooling{true};

	UPROPERTY(config, EditAnywhere, Category = "Performance", meta = (ClampMin = "1"))
	int32 MaxPooledWidgetsPerType{20};

	// Auto-save settings
	UPROPERTY(config, EditAnywhere, Category = "Save System")
	bool bAutoSaveOnChange{true};

	UPROPERTY(config, EditAnywhere, Category = "Save System")
	FString DefaultSaveSlotName = TEXT("GameSettings");

	// Tag-based settings caching
	UPROPERTY(config, EditAnywhere, Category = "Performance")
	bool bEnableTagCaching{true};

	UPROPERTY(Config, EditAnywhere, Category = "Configuration",
		meta=(AllowedClasses = "MCore_UIThemeDataAsset_Base"))
	TSoftObjectPtr<UMCore_UIThemeDataAsset_Base> CurrentThemeAsset;

	UPROPERTY(Config, EditAnywhere, Category = "WidgetClasses")
	TMap<EMCore_SettingType, TSoftClassPtr<UUserWidget>> WidgetClassOverrides;
};
