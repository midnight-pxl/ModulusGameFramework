// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CommonUI/Public/Input/CommonUIInputSettings.h"
#include "CoreUISystem/CoreGameSettings/MCore_GameSettingType.h"
#include "Engine/DeveloperSettings.h"
#include "MCore_CommonUISettings.generated.h"

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

	// Default assets (for the provided templates)
	UPROPERTY(config, EditAnywhere, Category = "Default Assets")
	TSoftObjectPtr<UCommonUIInputSettings> DefaultInputData;
	
	// Performance settings
	UPROPERTY(config, EditAnywhere, Category = "Performance")
	bool bPreloadCommonWidgets{true};

	UPROPERTY(config, EditAnywhere, Category = "Performance")
	float AsyncLoadTimeoutSeconds{3.0f};

	// Default settings menu configuration
	UPROPERTY(config, EditAnywhere, Category = "Default Configuration")
	FMCore_SettingsConfiguration DefaultSettingsConfiguration;

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

	// NEW: Tag-based settings caching
	UPROPERTY(config, EditAnywhere, Category = "Performance")
	bool bEnableTagCaching{true};

	// Helper to get widget class by setting type
	TSoftClassPtr<UUserWidget> GetWidgetClassForType(EMCore_SettingType SettingType) const;
	
};
