// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SModulusHubTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModulusHubTab) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	static const FName TabId;

private:
	// Settings Tab
	FReply OnResetSettingsClicked();
	FReply OnExportSettingsClicked();
	FReply OnImportSettingsClicked();
	FReply OnValidateSettingsClicked();

	// Ecosystem Tab
	void RefreshInstalledPlugins();

	// Documentation Tab
	FReply OnQuickStartClicked();
	FReply OnAPIReferenceClicked();

	// UI Update Helpers
	void UpdatePluginListUI();
	void UpdateValidationResultsUI();
	void OpenURL(const FString& URL);

	// Validation state
	TArray<FString> ValidationMessages;
	bool bLastValidationPassed = false;

	// Plugin data
	TArray<FString> InstalledPluginNames;
	TArray<bool> InstalledPluginEnabled;

	// UI References
	TSharedPtr<SVerticalBox> PluginListBox;
	TSharedPtr<SVerticalBox> ValidationResultsBox;
};
