// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreData/CoreStructEnums/EventStructsEnums/MCore_EventData.h"
#include "Engine/DeveloperSettings.h"
#include "MCore_ProjectSettings.generated.h"

/**
 * 
 */
UCLASS(config=ModulusGameFramework, DefaultConfig, meta=(DisplayName = "Modulus Game Framework"))
class MODULUSCORE_API UMCore_ProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UMCore_ProjectSettings();

	static const UMCore_ProjectSettings* Get()
	{
		return GetDefault<UMCore_ProjectSettings>();
	}

	// UDeveloperSettings interface
	virtual FName GetCategoryName() const override { return TEXT("Game"); }
	virtual FName GetSectionName() const override { return TEXT("Modulus Game Framework"); }
	
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
	UPROPERTY(config, EditAnywhere, Category = "Performance", meta = (ClampMin = "4", ClampMax = "25"))
	int32 MaxPooledObjectsPerType{10};

	/** Event System Configuration */
	UPROPERTY(EditAnywhere, Config, Category="Event System")
	EMCore_ValidationStrictness DefaultValidationStrictness = EMCore_ValidationStrictness::Balanced;

	UPROPERTY(EditAnywhere, Config, Category="Performance")
	bool bEnableUIScaling{true};

	UPROPERTY(EditAnywhere, Config, Category="UI Framework")
	float DefaultUIAnimationSpeed{0.3f};
    
	UPROPERTY(EditAnywhere, Config, Category="Performance", meta=(ClampMin="0.5", ClampMax="3.0"))
	float MaxUIScaleRange{2.0f};
	
#if WITH_EDITOR
    virtual FText GetSectionText() const override { return NSLOCTEXT("ModulusSettings", "SectionText", "Modulus Game Framework"); }
    virtual FText GetSectionDescription() const override { return NSLOCTEXT("ModulusSettings", "SectionDescription", "Configure the Modulus Game Framework settings"); }
#endif
	
};
