// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "SModulusHubTab.h"
#include "ModulusEditorStyle.h"
#include "CoreEditorLogging/LogModulusEditor.h"
#include "CoreData/Settings/MCore_ProjectSettings.h"
#include "CoreData/Settings/MCore_CommonUISettings.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Docking/SDockTab.h"
#include "Misc/FileHelper.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IPluginManager.h"
#include "HAL/PlatformProcess.h"
#include "Json.h"
#include "JsonUtilities.h"

const FName SModulusHubTab::TabId = FName(TEXT("ModulusHubTab"));

#define LOCTEXT_NAMESPACE "SModulusHubTab"

namespace ModulusHubLayout
{
	const FMargin ContentPadding(16.0f);
	const FMargin SectionPadding(0.0f, 8.0f);
	const FMargin ButtonPadding(0.0f, 0.0f, 8.0f, 0.0f);
	const FMargin ItemPadding(8.0f, 4.0f);
	const float SectionSpacing = 24.0f;
	const float HeaderHeight = 60.0f;
}

void SModulusHubTab::Construct(const FArguments& InArgs)
{
	RefreshInstalledPlugins();

	const ISlateStyle& Style = FModulusEditorStyle::Get();

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(Style.GetBrush("ModulusEditor.Background"))
		.Padding(0.0f)
		[
			SNew(SVerticalBox)

			// Header
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
				.BorderImage(Style.GetBrush("ModulusEditor.HeaderBox"))
				.Padding(ModulusHubLayout::ContentPadding)
				[
					SNew(SHorizontalBox)

					// Icon placeholder
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.Padding(0.0f, 0.0f, 12.0f, 0.0f)
					[
						SNew(SBox)
						.WidthOverride(32.0f)
						.HeightOverride(32.0f)
						[
							SNew(SImage)
							.Image(Style.GetBrush("ModulusEditor.ModulusIcon.Large"))
						]
					]

					// Title and subtitle
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					.VAlign(VAlign_Center)
					[
						SNew(SVerticalBox)

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("HubTitle", "Modulus Game Framework"))
							.TextStyle(&Style, "ModulusEditor.Text.Header")
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("HubSubtitle", "Central hub for framework configuration and tools"))
							.TextStyle(&Style, "ModulusEditor.Text.Muted")
						]
					]
				]
			]

			// Main Content
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SScrollBox)

				+ SScrollBox::Slot()
				.Padding(ModulusHubLayout::ContentPadding)
				[
					SNew(SVerticalBox)

					// ============================================
					// Settings Section
					// ============================================
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(ModulusHubLayout::SectionPadding)
					[
						SNew(SBorder)
						.BorderImage(Style.GetBrush("ModulusEditor.RoundedBox"))
						.Padding(ModulusHubLayout::ContentPadding)
						[
							SNew(SVerticalBox)

							// Section Header
							+ SVerticalBox::Slot()
							.AutoHeight()
							.Padding(0.0f, 0.0f, 0.0f, 12.0f)
							[
								SNew(SHorizontalBox)

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.VAlign(VAlign_Center)
								.Padding(0.0f, 0.0f, 8.0f, 0.0f)
								[
									SNew(SBox)
									.WidthOverride(16.0f)
									.HeightOverride(16.0f)
									[
										SNew(SImage)
										.Image(Style.GetBrush(FModulusEditorStyle::SettingsIconName))
									]
								]

								+ SHorizontalBox::Slot()
								.FillWidth(1.0f)
								.VAlign(VAlign_Center)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("SettingsHeader", "Settings"))
									.TextStyle(&Style, "ModulusEditor.Text.SectionHeader")
								]
							]

							// Settings Buttons Row
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SHorizontalBox)

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(ModulusHubLayout::ButtonPadding)
								[
									SNew(SButton)
									.ButtonStyle(&Style, "ModulusEditor.Button")
									.OnClicked(this, &SModulusHubTab::OnResetSettingsClicked)
									.ToolTipText(LOCTEXT("ResetTooltip", "Reset all Modulus settings to their default values"))
									[
										SNew(STextBlock)
										.Text(LOCTEXT("ResetBtn", "Reset All"))
									]
								]

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(ModulusHubLayout::ButtonPadding)
								[
									SNew(SButton)
									.ButtonStyle(&Style, "ModulusEditor.Button")
									.OnClicked(this, &SModulusHubTab::OnExportSettingsClicked)
									.ToolTipText(LOCTEXT("ExportTooltip", "Export current settings to a JSON file"))
									[
										SNew(STextBlock)
										.Text(LOCTEXT("ExportBtn", "Export"))
									]
								]

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(ModulusHubLayout::ButtonPadding)
								[
									SNew(SButton)
									.ButtonStyle(&Style, "ModulusEditor.Button")
									.OnClicked(this, &SModulusHubTab::OnImportSettingsClicked)
									.ToolTipText(LOCTEXT("ImportTooltip", "Import settings from a JSON file"))
									[
										SNew(STextBlock)
										.Text(LOCTEXT("ImportBtn", "Import"))
									]
								]

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(0.0f)
								[
									SNew(SButton)
									.ButtonStyle(&Style, "ModulusEditor.Button.Primary")
									.OnClicked(this, &SModulusHubTab::OnValidateSettingsClicked)
									.ToolTipText(LOCTEXT("ValidateTooltip", "Validate current settings configuration"))
									[
										SNew(STextBlock)
										.Text(LOCTEXT("ValidateBtn", "Validate"))
									]
								]
							]

							// Validation Results
							+ SVerticalBox::Slot()
							.AutoHeight()
							.Padding(0.0f, 12.0f, 0.0f, 0.0f)
							[
								SAssignNew(ValidationResultsBox, SVerticalBox)
							]
						]
					]

					// ============================================
					// Ecosystem Section
					// ============================================
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(ModulusHubLayout::SectionPadding)
					[
						SNew(SBorder)
						.BorderImage(Style.GetBrush("ModulusEditor.RoundedBox"))
						.Padding(ModulusHubLayout::ContentPadding)
						[
							SNew(SVerticalBox)

							// Section Header
							+ SVerticalBox::Slot()
							.AutoHeight()
							.Padding(0.0f, 0.0f, 0.0f, 12.0f)
							[
								SNew(SHorizontalBox)

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.VAlign(VAlign_Center)
								.Padding(0.0f, 0.0f, 8.0f, 0.0f)
								[
									SNew(SBox)
									.WidthOverride(16.0f)
									.HeightOverride(16.0f)
									[
										SNew(SImage)
										.Image(Style.GetBrush(FModulusEditorStyle::EcosystemIconName))
									]
								]

								+ SHorizontalBox::Slot()
								.FillWidth(1.0f)
								.VAlign(VAlign_Center)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("EcosystemHeader", "Ecosystem"))
									.TextStyle(&Style, "ModulusEditor.Text.SectionHeader")
								]

								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew(SButton)
									.ButtonStyle(&Style, "ModulusEditor.Button")
									.OnClicked_Lambda([this]()
									{
										RefreshInstalledPlugins();
										UpdatePluginListUI();
										return FReply::Handled();
									})
									[
										SNew(STextBlock)
										.Text(LOCTEXT("RefreshBtn", "Refresh"))
									]
								]
							]

							// Plugin List
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SAssignNew(PluginListBox, SVerticalBox)
							]
						]
					]

					// ============================================
					// Documentation Section
					// ============================================
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(ModulusHubLayout::SectionPadding)
					[
						SNew(SBorder)
						.BorderImage(Style.GetBrush("ModulusEditor.RoundedBox"))
						.Padding(ModulusHubLayout::ContentPadding)
						[
							SNew(SVerticalBox)

							// Section Header
							+ SVerticalBox::Slot()
							.AutoHeight()
							.Padding(0.0f, 0.0f, 0.0f, 12.0f)
							[
								SNew(SHorizontalBox)

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.VAlign(VAlign_Center)
								.Padding(0.0f, 0.0f, 8.0f, 0.0f)
								[
									SNew(SBox)
									.WidthOverride(16.0f)
									.HeightOverride(16.0f)
									[
										SNew(SImage)
										.Image(Style.GetBrush(FModulusEditorStyle::DocsIconName))
									]
								]

								+ SHorizontalBox::Slot()
								.FillWidth(1.0f)
								.VAlign(VAlign_Center)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("DocsHeader", "Documentation"))
									.TextStyle(&Style, "ModulusEditor.Text.SectionHeader")
								]
							]

							// Doc Buttons
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SHorizontalBox)

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(ModulusHubLayout::ButtonPadding)
								[
									SNew(SButton)
									.ButtonStyle(&Style, "ModulusEditor.Button")
									.OnClicked(this, &SModulusHubTab::OnQuickStartClicked)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("QuickStartBtn", "Quick Start Guide"))
									]
								]

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(0.0f)
								[
									SNew(SButton)
									.ButtonStyle(&Style, "ModulusEditor.Button")
									.OnClicked(this, &SModulusHubTab::OnAPIReferenceClicked)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("APIRefBtn", "API Reference"))
									]
								]
							]
						]
					]

					// Bottom spacer
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SSpacer)
						.Size(FVector2D(1.0f, 16.0f))
					]
				]
			]

			// Footer
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
				.BorderImage(Style.GetBrush("ModulusEditor.Separator"))
				.Padding(FMargin(ModulusHubLayout::ContentPadding.Left, 8.0f, ModulusHubLayout::ContentPadding.Right, 8.0f))
				[
					SNew(STextBlock)
					.Text(LOCTEXT("FooterText", "Modulus Game Framework - Midnight Pixel Studio"))
					.TextStyle(&Style, "ModulusEditor.Text.Muted")
				]
			]
		]
	];

	// Initialize plugin list UI
	UpdatePluginListUI();

	UE_LOG(LogModulusEditor, Log, TEXT("Modulus Hub opened"));
}

void SModulusHubTab::UpdatePluginListUI()
{
	if (!PluginListBox.IsValid())
	{
		return;
	}

	const ISlateStyle& Style = FModulusEditorStyle::Get();
	PluginListBox->ClearChildren();

	if (InstalledPluginNames.Num() == 0)
	{
		PluginListBox->AddSlot()
		.AutoHeight()
		.Padding(ModulusHubLayout::ItemPadding)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("NoPlugins", "No Modulus plugins found"))
			.TextStyle(&Style, "ModulusEditor.Text.Muted")
		];
		return;
	}

	for (int32 i = 0; i < InstalledPluginNames.Num(); ++i)
	{
		const bool bEnabled = InstalledPluginEnabled[i];

		PluginListBox->AddSlot()
		.AutoHeight()
		.Padding(ModulusHubLayout::ItemPadding)
		[
			SNew(SBorder)
			.BorderImage(Style.GetBrush("ModulusEditor.AccentBox"))
			.Padding(FMargin(12.0f, 8.0f))
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString(InstalledPluginNames[i]))
					.TextStyle(&Style, "ModulusEditor.Text.Normal")
				]

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(bEnabled ? LOCTEXT("Enabled", "Enabled") : LOCTEXT("Disabled", "Disabled"))
					.ColorAndOpacity(bEnabled ? FModulusEditorStyle::GetSuccessColor() : FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f)))
				]
			]
		];
	}
}

void SModulusHubTab::UpdateValidationResultsUI()
{
	if (!ValidationResultsBox.IsValid())
	{
		return;
	}

	const ISlateStyle& Style = FModulusEditorStyle::Get();
	ValidationResultsBox->ClearChildren();

	// Status header
	ValidationResultsBox->AddSlot()
	.AutoHeight()
	.Padding(0.0f, 0.0f, 0.0f, 8.0f)
	[
		SNew(STextBlock)
		.Text(bLastValidationPassed ? LOCTEXT("ValidationPassed", "Validation: PASSED") : LOCTEXT("ValidationFailed", "Validation: FAILED"))
		.TextStyle(&Style, "ModulusEditor.Text.SubHeader")
		.ColorAndOpacity(bLastValidationPassed ? FModulusEditorStyle::GetSuccessColor() : FModulusEditorStyle::GetErrorColor())
	];

	// Messages
	for (const FString& Message : ValidationMessages)
	{
		FLinearColor MessageColor = FLinearColor::White;
		if (Message.StartsWith(TEXT("Error")))
		{
			MessageColor = FModulusEditorStyle::GetErrorColor();
		}
		else if (Message.StartsWith(TEXT("Warning")))
		{
			MessageColor = FModulusEditorStyle::GetWarningColor();
		}
		else
		{
			MessageColor = FModulusEditorStyle::GetSuccessColor();
		}

		ValidationResultsBox->AddSlot()
		.AutoHeight()
		.Padding(12.0f, 2.0f, 0.0f, 2.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Message))
			.ColorAndOpacity(FSlateColor(MessageColor))
		];
	}
}

FReply SModulusHubTab::OnResetSettingsClicked()
{
	if (UMCore_ProjectSettings* Settings = GetMutableDefault<UMCore_ProjectSettings>())
	{
		Settings->bAutoInitializePlugins = true;
		Settings->EnabledFeaturePlugins.Empty();
		Settings->bEnableWidgetPooling = true;
		Settings->MaxPooledObjectsPerType = 10;
		Settings->DefaultValidationStrictness = EMCore_ValidationStrictness::Balanced;
		Settings->bEnableUIScaling = true;
		Settings->DefaultUIAnimationSpeed = 0.3f;
		Settings->MaxUIScaleRange = 2.0f;
		Settings->SaveConfig();
	}

	if (UMCore_CommonUISettings* UISettings = GetMutableDefault<UMCore_CommonUISettings>())
	{
		UISettings->CurrentThemeAsset.Reset();
		UISettings->AvailableThemes.Empty();
		UISettings->DefaultThemeIndex = 0;
		UISettings->bEnableTagCaching = true;
		UISettings->WidgetClassOverrides.Empty();
		UISettings->CurrentDefaultSettings.Reset();
		UISettings->SaveConfig();
	}

	UE_LOG(LogModulusEditor, Log, TEXT("[Reset Settings] All Modulus settings reset to defaults"));
	return FReply::Handled();
}

FReply SModulusHubTab::OnExportSettingsClicked()
{
	IDesktopPlatform* Desktop = FDesktopPlatformModule::Get();
	if (!Desktop)
	{
		UE_LOG(LogModulusEditor, Warning, TEXT("[Export] Desktop platform unavailable"));
		return FReply::Handled();
	}

	TArray<FString> Files;
	const bool bOpened = Desktop->SaveFileDialog(
		FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
		TEXT("Export Modulus Settings"),
		FPaths::ProjectSavedDir(),
		TEXT("ModulusSettings.json"),
		TEXT("JSON (*.json)|*.json"),
		0,
		Files);

	if (!bOpened || Files.Num() == 0)
	{
		return FReply::Handled();
	}

	TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();

	if (const UMCore_ProjectSettings* ProjectSettings = UMCore_ProjectSettings::Get())
	{
		TSharedPtr<FJsonObject> PSJson = MakeShared<FJsonObject>();
		PSJson->SetBoolField(TEXT("bAutoInitializePlugins"), ProjectSettings->bAutoInitializePlugins);
		PSJson->SetBoolField(TEXT("bEnableWidgetPooling"), ProjectSettings->bEnableWidgetPooling);
		PSJson->SetNumberField(TEXT("MaxPooledObjectsPerType"), ProjectSettings->MaxPooledObjectsPerType);
		PSJson->SetNumberField(TEXT("DefaultValidationStrictness"), static_cast<int32>(ProjectSettings->DefaultValidationStrictness));
		PSJson->SetBoolField(TEXT("bEnableUIScaling"), ProjectSettings->bEnableUIScaling);
		PSJson->SetNumberField(TEXT("DefaultUIAnimationSpeed"), ProjectSettings->DefaultUIAnimationSpeed);
		PSJson->SetNumberField(TEXT("MaxUIScaleRange"), ProjectSettings->MaxUIScaleRange);

		TArray<TSharedPtr<FJsonValue>> PluginsArray;
		for (const FString& Plugin : ProjectSettings->EnabledFeaturePlugins)
		{
			PluginsArray.Add(MakeShared<FJsonValueString>(Plugin));
		}
		PSJson->SetArrayField(TEXT("EnabledFeaturePlugins"), PluginsArray);

		Root->SetObjectField(TEXT("ProjectSettings"), PSJson);
	}

	if (const UMCore_CommonUISettings* UISettings = UMCore_CommonUISettings::Get())
	{
		TSharedPtr<FJsonObject> UIJson = MakeShared<FJsonObject>();
		UIJson->SetNumberField(TEXT("DefaultThemeIndex"), UISettings->DefaultThemeIndex);
		UIJson->SetBoolField(TEXT("bEnableTagCaching"), UISettings->bEnableTagCaching);
		UIJson->SetStringField(TEXT("DefaultSaveSlotName"), UISettings->DefaultSaveSlotName);
		Root->SetObjectField(TEXT("UISettings"), UIJson);
	}

	Root->SetStringField(TEXT("ExportVersion"), TEXT("1.0"));
	Root->SetStringField(TEXT("ExportDate"), FDateTime::Now().ToString());

	FString Output;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Output);
	FJsonSerializer::Serialize(Root.ToSharedRef(), Writer);

	if (FFileHelper::SaveStringToFile(Output, *Files[0]))
	{
		UE_LOG(LogModulusEditor, Log, TEXT("[Export] Saved to %s"), *Files[0]);
	}
	else
	{
		UE_LOG(LogModulusEditor, Error, TEXT("[Export] Failed to write file"));
	}

	return FReply::Handled();
}

FReply SModulusHubTab::OnImportSettingsClicked()
{
	IDesktopPlatform* Desktop = FDesktopPlatformModule::Get();
	if (!Desktop)
	{
		UE_LOG(LogModulusEditor, Warning, TEXT("[Import] Desktop platform unavailable"));
		return FReply::Handled();
	}

	TArray<FString> Files;
	const bool bOpened = Desktop->OpenFileDialog(
		FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
		TEXT("Import Modulus Settings"),
		FPaths::ProjectSavedDir(),
		TEXT(""),
		TEXT("JSON (*.json)|*.json"),
		0,
		Files);

	if (!bOpened || Files.Num() == 0)
	{
		return FReply::Handled();
	}

	FString Content;
	if (!FFileHelper::LoadFileToString(Content, *Files[0]))
	{
		UE_LOG(LogModulusEditor, Error, TEXT("[Import] Failed to read file"));
		return FReply::Handled();
	}

	TSharedPtr<FJsonObject> Root;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);
	if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
	{
		UE_LOG(LogModulusEditor, Error, TEXT("[Import] Invalid JSON format"));
		return FReply::Handled();
	}

	const TSharedPtr<FJsonObject>* ProjectJson = nullptr;
	if (Root->TryGetObjectField(TEXT("ProjectSettings"), ProjectJson) && ProjectJson)
	{
		if (UMCore_ProjectSettings* PS = GetMutableDefault<UMCore_ProjectSettings>())
		{
			PS->bAutoInitializePlugins = (*ProjectJson)->GetBoolField(TEXT("bAutoInitializePlugins"));
			PS->bEnableWidgetPooling = (*ProjectJson)->GetBoolField(TEXT("bEnableWidgetPooling"));
			PS->MaxPooledObjectsPerType = (*ProjectJson)->GetIntegerField(TEXT("MaxPooledObjectsPerType"));
			PS->DefaultValidationStrictness = static_cast<EMCore_ValidationStrictness>((*ProjectJson)->GetIntegerField(TEXT("DefaultValidationStrictness")));
			PS->bEnableUIScaling = (*ProjectJson)->GetBoolField(TEXT("bEnableUIScaling"));
			PS->DefaultUIAnimationSpeed = (*ProjectJson)->GetNumberField(TEXT("DefaultUIAnimationSpeed"));
			PS->MaxUIScaleRange = (*ProjectJson)->GetNumberField(TEXT("MaxUIScaleRange"));

			const TArray<TSharedPtr<FJsonValue>>* PluginsArray = nullptr;
			if ((*ProjectJson)->TryGetArrayField(TEXT("EnabledFeaturePlugins"), PluginsArray) && PluginsArray)
			{
				PS->EnabledFeaturePlugins.Empty();
				for (const TSharedPtr<FJsonValue>& Value : *PluginsArray)
				{
					PS->EnabledFeaturePlugins.Add(Value->AsString());
				}
			}

			PS->SaveConfig();
		}
	}

	const TSharedPtr<FJsonObject>* UIJson = nullptr;
	if (Root->TryGetObjectField(TEXT("UISettings"), UIJson) && UIJson)
	{
		if (UMCore_CommonUISettings* UISettings = GetMutableDefault<UMCore_CommonUISettings>())
		{
			UISettings->DefaultThemeIndex = (*UIJson)->GetIntegerField(TEXT("DefaultThemeIndex"));
			UISettings->bEnableTagCaching = (*UIJson)->GetBoolField(TEXT("bEnableTagCaching"));

			FString SaveSlotName;
			if ((*UIJson)->TryGetStringField(TEXT("DefaultSaveSlotName"), SaveSlotName))
			{
				UISettings->DefaultSaveSlotName = SaveSlotName;
			}

			UISettings->SaveConfig();
		}
	}

	UE_LOG(LogModulusEditor, Log, TEXT("[Import] Loaded from %s"), *Files[0]);
	return FReply::Handled();
}

FReply SModulusHubTab::OnValidateSettingsClicked()
{
	ValidationMessages.Empty();
	bLastValidationPassed = true;

	const UMCore_CommonUISettings* UISettings = UMCore_CommonUISettings::Get();
	const UMCore_ProjectSettings* ProjectSettings = UMCore_ProjectSettings::Get();

	if (!UISettings)
	{
		ValidationMessages.Add(TEXT("Error: UMCore_CommonUISettings not found"));
		bLastValidationPassed = false;
	}
	else
	{
		if (UISettings->CurrentThemeAsset.IsNull())
		{
			ValidationMessages.Add(TEXT("Warning: No CurrentThemeAsset configured"));
		}

		if (UISettings->AvailableThemes.Num() == 0)
		{
			ValidationMessages.Add(TEXT("Warning: AvailableThemes array is empty"));
		}
		else if (UISettings->DefaultThemeIndex < 0 || UISettings->DefaultThemeIndex >= UISettings->AvailableThemes.Num())
		{
			ValidationMessages.Add(FString::Printf(TEXT("Error: DefaultThemeIndex (%d) out of range [0, %d)"),
				UISettings->DefaultThemeIndex, UISettings->AvailableThemes.Num()));
			bLastValidationPassed = false;
		}

		if (UISettings->CurrentDefaultSettings.IsNull())
		{
			ValidationMessages.Add(TEXT("Warning: No CurrentDefaultSettings configured"));
		}
	}

	if (!ProjectSettings)
	{
		ValidationMessages.Add(TEXT("Error: UMCore_ProjectSettings not found"));
		bLastValidationPassed = false;
	}

	if (ValidationMessages.Num() == 0)
	{
		ValidationMessages.Add(TEXT("All checks passed"));
	}

	UpdateValidationResultsUI();

	UE_LOG(LogModulusEditor, Log, TEXT("[Validate] %s"), bLastValidationPassed ? TEXT("PASSED") : TEXT("FAILED"));
	return FReply::Handled();
}

void SModulusHubTab::RefreshInstalledPlugins()
{
	InstalledPluginNames.Empty();
	InstalledPluginEnabled.Empty();

	TArray<TSharedRef<IPlugin>> AllPlugins = IPluginManager::Get().GetDiscoveredPlugins();
	for (const TSharedRef<IPlugin>& Plugin : AllPlugins)
	{
		if (Plugin->GetName().Contains(TEXT("Modulus")))
		{
			InstalledPluginNames.Add(Plugin->GetName());
			InstalledPluginEnabled.Add(Plugin->IsEnabled());
		}
	}

	UE_LOG(LogModulusEditor, Log, TEXT("[Refresh Plugins] Found %d Modulus plugin(s)"), InstalledPluginNames.Num());
}

FReply SModulusHubTab::OnQuickStartClicked()
{
	// OpenURL(TEXT("https://github.com/MidnightPixelStudios/ModulusGameFramework/wiki/Quick-Start"));
	UE_LOG(LogModulusEditor, Log, TEXT("[Docs] Opened Quick Start Guide"));
	return FReply::Handled();
}

FReply SModulusHubTab::OnAPIReferenceClicked()
{
	// OpenURL(TEXT("https://github.com/MidnightPixelStudios/ModulusGameFramework/wiki/API-Reference"));
	UE_LOG(LogModulusEditor, Log, TEXT("[Docs] Opened API Reference"));
	return FReply::Handled();
}

void SModulusHubTab::OpenURL(const FString& URL)
{
	if (!URL.IsEmpty())
	{
		FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
	}
}

#undef LOCTEXT_NAMESPACE
