// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
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
	UPROPERTY(config, EditAnywhere, Category = "Layer Configuration")
	FString GameLayerTag = TEXT("UI.Layer.Game");
    
	UPROPERTY(config, EditAnywhere, Category = "Layer Configuration")
	FString GameMenuLayerTag = TEXT("UI.Layer.GameMenu");
    
	UPROPERTY(config, EditAnywhere, Category = "Layer Configuration")
	FString MenuLayerTag = TEXT("UI.Layer.Menu");
    
	UPROPERTY(config, EditAnywhere, Category = "Layer Configuration")
	FString ModalLayerTag = TEXT("UI.Layer.Modal");

	// CommonUI auto-setup
	UPROPERTY(config, EditAnywhere, Category = "Setup")
	bool bAutoConfigureCommonUIOnStartup = true;

	// Default assets (for the provided templates)
	UPROPERTY(config, EditAnywhere, Category = "Default Assets")
	TSoftObjectPtr<UCommonUIInputData> DefaultInputData;

	// Performance settings
	UPROPERTY(config, EditAnywhere, Category = "Performance")
	bool bPreloadCommonWidgets = true;

	UPROPERTY(config, EditAnywhere, Category = "Performance")
	float AsyncLoadTimeoutSeconds = 5.0f;
};
