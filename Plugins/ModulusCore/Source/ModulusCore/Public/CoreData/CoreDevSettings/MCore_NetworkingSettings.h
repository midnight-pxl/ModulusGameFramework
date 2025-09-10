// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MCore_NetworkingSettings.generated.h"

/**
 * 
 */
UCLASS(Config=game, DefaultConfig, meta=(DisplayName="Modulus Networking Settings"))
class MODULUSCORE_API UMCore_NetworkingSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	// Core networking configuration
	UPROPERTY(config, EditAnywhere, Category="Iris", 
			  meta = (ToolTip="Enable Iris experimental replication system. Requires UE 5.6+ and project configuration."))
	bool bUseIrisReplication{false};
    
	UPROPERTY(config, EditAnywhere, Category="Performance", 
			  meta = (EditCondition="!bUseIrisReplication"))
	bool bUsePushModelNetworking{true};
    
	UPROPERTY(config, EditAnywhere, Category="Performance")
	bool bEnableNetworkPerformanceTracking{true};
    
	UPROPERTY(config, EditAnywhere, Category="Debug")
	bool bEnableVerboseNetworkLogging{false};
    
	// Iris-specific settings (only relevant when Iris is enabled)
	UPROPERTY(config, EditAnywhere, Category="Iris", 
			  meta = (EditCondition="bUseIrisReplication"))
	bool bUseIrisCustomStateStreaming{true};
    
	UPROPERTY(config, EditAnywhere, Category="Iris", 
			  meta = (EditCondition="bUseIrisReplication"))
	float IrisReplicationFrequency{30.0f};

	// Network authority validation levels
	UPROPERTY(config, EditAnywhere, Category="Security")
	bool bStrictNetworkValidation{true};
    
	// Runtime validation
	virtual void PostInitProperties() override;
    
private:
	void ValidateConfiguration() const;
};
