// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CommonUI/Public/Input/CommonUIInputSettings.h"
#include "Engine/DeveloperSettings.h"
#include "MCore_CommonUISettings.generated.h"

/**
 * 
 */
UCLASS(Config=ModulusGameFramework, DefaultConfig)
class MODULUSCORE_API UMCore_CommonUISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
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
	bool bPreloadCommonWidgets = true;

	UPROPERTY(config, EditAnywhere, Category = "Performance")
	float AsyncLoadTimeoutSeconds = 3.0f;
};
