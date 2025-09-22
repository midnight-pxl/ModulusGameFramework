// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CoreData/CoreStructEnums/UIStructsEnums/CoreGameSettings/MCore_GameSettingType.h"
#include "Engine/DeveloperSettings.h"
#include "MCore_CommonUISettings.generated.h"

class UMCore_DA_UITheme_Base;

/**
 * 
 */
UCLASS(Config=game, DefaultConfig, meta=(DisplayName="Modulus UI Settings"))
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
	
	UPROPERTY(config, EditAnywhere, Category = "Performance")
	bool bEnableWidgetPooling{true};

	UPROPERTY(config, EditAnywhere, Category = "Performance", meta = (ClampMin = "1"))
	int32 MaxPooledWidgetsPerType{12};

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
	TSoftObjectPtr<UMCore_DA_UITheme_Base> CurrentThemeAsset;

	UPROPERTY(Config, EditAnywhere, Category = "WidgetClasses")
	TMap<EMCore_SettingType, TSoftClassPtr<UUserWidget>> WidgetClassOverrides;

	// Current DataAsset for Default Settings values
	UPROPERTY(Config, EditAnywhere, Category = "Configuration",
		meta=(DisplayName = "Default Settings Config"))
	TSoftObjectPtr<class UMCore_DA_GameSettings_Base> CurrentDefaultSettings;
};
