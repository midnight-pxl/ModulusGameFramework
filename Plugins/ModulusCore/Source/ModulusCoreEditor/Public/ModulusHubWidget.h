// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "ModulusHubWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MODULUSCOREEDITOR_API UModulusHubWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
    UModulusHubWidget(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativeConstruct() override;

    /**
     * Settings Tab
     */
    
    /** Reset all Modulus settings to defaults */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ResetAllSettings();
    
    /** Export current settings to file */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ExportSettings();
    
    /** Import settings from file */
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ImportSettings();

    /**
     * Tools Tab
     */
    
    /** Generate default settings menu for current project */
    UFUNCTION(BlueprintCallable, Category = "Tools")
    void GenerateDefaultSettingsMenu();
    
    /** Validate current settings configuration */
    UFUNCTION(BlueprintCallable, Category = "Tools")
    void ValidateSettingsConfiguration();
    
    /** Open theme preview window */
    UFUNCTION(BlueprintCallable, Category = "Tools")
    void PreviewThemes();

    /**
     * Compliance Tab
     */
    
    /** Test Xbox accessibility compliance */
    UFUNCTION(BlueprintCallable, Category = "Compliance")
    void TestXboxCompliance();
    
    /** Test PlayStation accessibility compliance */
    UFUNCTION(BlueprintCallable, Category = "Compliance")
    void TestPlayStationCompliance();
    
    /** Test text contrast compliance */
    UFUNCTION(BlueprintCallable, Category = "Compliance")
    void TestTextContrast();
    
    /** Test colorblind accessibility */
    UFUNCTION(BlueprintCallable, Category = "Compliance")
    void TestColorblindAccessibility();
    
    /** Generate certification report */
    UFUNCTION(BlueprintCallable, Category = "Compliance")
    void GenerateCertificationReport();

    /**
     * Modulus Ecosystem Tab
     */
    
    /** Refresh installed plugins list */
    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void RefreshInstalledPlugins();
    
    /** Run quick setup for Modulus framework */
    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void RunQuickSetup();
    
    /** Validate plugin integration */
    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void ValidatePluginIntegration();

	/**
	 * Documentation Tab
	 */
    
    /** Open quick start guide */
    UFUNCTION(BlueprintCallable, Category = "Documentation")
    void OpenQuickStartGuide();
    
    /** Open API reference */
    UFUNCTION(BlueprintCallable, Category = "Documentation")
    void OpenAPIReference();

private:
    /** Update the interface with current plugin status */
    void UpdatePluginStatus();
    
    /** Display results in the compliance tab */
    void DisplayComplianceResults(const TArray<FString>& Results);
    
    /** Log action to output log */
    void LogAction(const FString& Action, const FString& LogResult);
};
