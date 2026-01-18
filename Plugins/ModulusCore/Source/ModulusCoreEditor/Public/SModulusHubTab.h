// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SCheckBox;

class SModulusHubTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModulusHubTab) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	static const FName TabId;

private:
	// Settings Section
	FReply OnOpenProjectSettingsClicked();
	void OnEventLoggingCheckStateChanged(ECheckBoxState NewState);
	void OnUIDebugOverlayCheckStateChanged(ECheckBoxState NewState);
	ECheckBoxState GetEventLoggingCheckState() const;
	ECheckBoxState GetUIDebugOverlayCheckState() const;

	// UI Theme Section
	FReply OnEditThemeAssetClicked();
	FText GetActiveThemeName() const;

	// Ecosystem Section
	void RefreshInstalledPlugins();

	// Documentation Section
	FReply OnQuickStartClicked();
	FReply OnAPIReferenceClicked();

	// UI Update Helpers
	void UpdatePluginListUI();
	void OpenURL(const FString& URL);

	// Plugin data
	TArray<FString> InstalledPluginNames;
	TArray<bool> InstalledPluginEnabled;

	// UI References
	TSharedPtr<SVerticalBox> PluginListBox;
	TSharedPtr<SCheckBox> EventLoggingCheckbox;
	TSharedPtr<SCheckBox> UIDebugOverlayCheckbox;
};
