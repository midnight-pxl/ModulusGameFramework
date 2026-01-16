// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "ModulusHubWidget.generated.h"

/**
 * Modulus Hub Widget - Central editor hub for Modulus Game Framework
 * Provides settings management, ecosystem overview, and documentation access.
 */
UCLASS(BlueprintType, Blueprintable)
class MODULUSCOREEDITOR_API UModulusHubWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UModulusHubWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	//=========================================================================
	// Settings Tab
	//=========================================================================

	/** Reset all Modulus settings to defaults */
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ResetAllSettings();

	/** Export current settings to JSON file */
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ExportSettings();

	/** Import settings from JSON file */
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ImportSettings();

	/** Validate current settings configuration */
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ValidateSettingsConfiguration();

	//=========================================================================
	// Ecosystem Tab
	//=========================================================================

	/** Refresh installed plugins list */
	UFUNCTION(BlueprintCallable, Category = "Ecosystem")
	void RefreshInstalledPlugins();

	//=========================================================================
	// Documentation Tab
	//=========================================================================

	/** Open quick start guide in browser */
	UFUNCTION(BlueprintCallable, Category = "Documentation")
	void OpenQuickStartGuide();

	/** Open API reference in browser */
	UFUNCTION(BlueprintCallable, Category = "Documentation")
	void OpenAPIReference();

	//=========================================================================
	// Blueprint-Bindable Properties
	//=========================================================================

	/** Installed Modulus plugin names (for Blueprint binding) */
	UPROPERTY(BlueprintReadOnly, Category = "Ecosystem")
	TArray<FString> InstalledPluginNames;

	/** Plugin enabled states (parallel array to InstalledPluginNames) */
	UPROPERTY(BlueprintReadOnly, Category = "Ecosystem")
	TArray<bool> InstalledPluginEnabled;

	/** Whether last validation passed */
	UPROPERTY(BlueprintReadOnly, Category = "Settings")
	bool bLastValidationPassed{false};

	/** Validation result messages */
	UPROPERTY(BlueprintReadOnly, Category = "Settings")
	TArray<FString> ValidationMessages;

private:
	/** Get list of installed Modulus plugins */
	TArray<FString> GetInstalledModulusPlugins();

	/** Update plugin status data */
	void UpdatePluginStatus();

	/** Open URL in default browser */
	void OpenURL(const FString& URL);
};
