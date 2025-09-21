// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "MCore_FrameworkSettings.generated.h"

/**
 * 
 */
UCLASS(config=Game, DefaultConfig, meta=(DisplayName = "Modulus Game Framework"))
class MODULUSCORE_API UMCore_FrameworkSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UMCore_FrameworkSettings();


	/**
	 * Core Framework Settings
	 */
	
	/** Enable verbose logging for all Modulus systems */
	UPROPERTY(config, EditAnywhere, Category = "Framework", meta = (DisplayName = "Enable Verbose Logging"))
	bool bEnableVerboseLogging{false};

	/**
	 * Plugin Management
	 */
    
	/** Automatically initialize Modulus plugins on startup */
	UPROPERTY(config, EditAnywhere, Category = "Plugins")
	bool bAutoInitializePlugins{true};
    
	/** List of enabled Modulus feature plugins */
	UPROPERTY(config, EditAnywhere, Category = "Plugins")
	TArray<FString> EnabledFeaturePlugins;

	/**
	 * Performance Settings
	 */
    
	/** Enable object pooling for UI widgets */
	UPROPERTY(config, EditAnywhere, Category = "Performance")
	bool bEnableWidgetPooling{true};
    
	/** Maximum pooled objects per type */
	UPROPERTY(config, EditAnywhere, Category = "Performance", meta = (ClampMin = "1", ClampMax = "100"))
	int32 MaxPooledObjectsPerType{12};

    // UDeveloperSettings interface
    virtual FName GetCategoryName() const override { return TEXT("Game"); }
    virtual FName GetSectionName() const override { return TEXT("Modulus Game Framework"); }

#if WITH_EDITOR
    virtual FText GetSectionText() const override { return NSLOCTEXT("ModulusSettings", "SectionText", "Modulus Game Framework"); }
    virtual FText GetSectionDescription() const override { return NSLOCTEXT("ModulusSettings", "SectionDescription", "Configure the Modulus Game Framework settings"); }
#endif
	
};
