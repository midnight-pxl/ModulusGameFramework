// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MCore_DA_GameSettings_Base.generated.h"

/**
 * Comprehensive game settings data asset
 * Provides foundation for all ModulusCore settings functionality
 */
UCLASS()
class MODULUSCORE_API UMCore_DA_GameSettings_Base : public UDataAsset
{
	GENERATED_BODY()

public:
    UMCore_DA_GameSettings_Base();
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FMCore_GraphicsSettingsData GraphicsSettings;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FMCore_AudioSettingsData AudioSettings;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FMCore_ControlsSettingsData ControlsSettings;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Accessibility")
    FMCore_AccessibilitySettings AccessibilitySettings;
    
    // Platform-specific validation data
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<FString, FMCore_PlatformCapabilities> PlatformCapabilities;

    // Platform defaults - simple data only
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform Support")
    TArray<FIntPoint> SupportedResolutions;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform Support")
    TArray<FText> GraphicsPresets;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform Support")
    TArray<FText> WindowModes;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform Support") 
    TArray<FText> AntiAliasingOptions;

    // Simple getters for template usage
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings Categories")
    const TArray<FText>& GetGraphicsCategories() const { return GraphicsCategories; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings Categories")
    const TArray<FText>& GetAudioCategories() const { return AudioCategories; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings Categories")
    const TArray<FText>& GetControlCategories() const { return ControlCategories; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings Categories")
    const TArray<FText>& GetUICategories() const { return UICategories; }

private:
    /**
     * Initialize default category organization and platform defaults
     */
    void InitializeDefaults();
};
