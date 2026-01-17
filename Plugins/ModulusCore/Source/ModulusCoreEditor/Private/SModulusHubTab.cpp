// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "SModulusHubTab.h"
#include "ModulusEditorStyle.h"
#include "CoreEditorLogging/LogModulusEditor.h"
#include "CoreData/DevSettings/MCore_CoreSettings.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Interfaces/IPluginManager.h"
#include "ISettingsModule.h"
#include "HAL/PlatformProcess.h"

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
							.Text(LOCTEXT("HubSubtitle", "Central hub for framework tools and documentation"))
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
									.Text(LOCTEXT("EcosystemHeader", "Installed Plugins"))
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
									.Text(LOCTEXT("SettingsHeader", "Debug Settings"))
									.TextStyle(&Style, "ModulusEditor.Text.SectionHeader")
								]

								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew(SButton)
									.ButtonStyle(&Style, "ModulusEditor.Button")
									.OnClicked(this, &SModulusHubTab::OnOpenProjectSettingsClicked)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("OpenSettingsBtn", "Open Project Settings"))
									]
								]
							]

							// Event Logging Toggle
							+ SVerticalBox::Slot()
							.AutoHeight()
							.Padding(0.0f, 4.0f)
							[
								SNew(SHorizontalBox)

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.VAlign(VAlign_Center)
								.Padding(0.0f, 0.0f, 8.0f, 0.0f)
								[
									SAssignNew(EventLoggingCheckbox, SCheckBox)
									.IsChecked(this, &SModulusHubTab::GetEventLoggingCheckState)
									.OnCheckStateChanged(this, &SModulusHubTab::OnEventLoggingCheckStateChanged)
								]

								+ SHorizontalBox::Slot()
								.FillWidth(1.0f)
								.VAlign(VAlign_Center)
								[
									SNew(SVerticalBox)

									+ SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(STextBlock)
										.Text(LOCTEXT("EventLoggingLabel", "Event System Logging"))
										.TextStyle(&Style, "ModulusEditor.Text.Normal")
									]

									+ SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(STextBlock)
										.Text(LOCTEXT("EventLoggingDesc", "Log all event broadcasts and subscriptions to Output Log"))
										.TextStyle(&Style, "ModulusEditor.Text.Muted")
									]
								]
							]

							// UI Debug Overlay Toggle
							+ SVerticalBox::Slot()
							.AutoHeight()
							.Padding(0.0f, 4.0f)
							[
								SNew(SHorizontalBox)

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.VAlign(VAlign_Center)
								.Padding(0.0f, 0.0f, 8.0f, 0.0f)
								[
									SAssignNew(UIDebugOverlayCheckbox, SCheckBox)
									.IsChecked(this, &SModulusHubTab::GetUIDebugOverlayCheckState)
									.OnCheckStateChanged(this, &SModulusHubTab::OnUIDebugOverlayCheckStateChanged)
								]

								+ SHorizontalBox::Slot()
								.FillWidth(1.0f)
								.VAlign(VAlign_Center)
								[
									SNew(SVerticalBox)

									+ SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(STextBlock)
										.Text(LOCTEXT("UIDebugOverlayLabel", "UI Layer Debug Overlay"))
										.TextStyle(&Style, "ModulusEditor.Text.Normal")
									]

									+ SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(STextBlock)
										.Text(LOCTEXT("UIDebugOverlayDesc", "Show debug overlay for CommonUI layer stack"))
										.TextStyle(&Style, "ModulusEditor.Text.Muted")
									]
								]
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

FReply SModulusHubTab::OnOpenProjectSettingsClicked()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule)
	{
		SettingsModule->ShowViewer("Project", "Game", "Modulus Core");
	}
	return FReply::Handled();
}

void SModulusHubTab::OnEventLoggingCheckStateChanged(ECheckBoxState NewState)
{
#if WITH_EDITORONLY_DATA
	UMCore_CoreSettings* Settings = GetMutableDefault<UMCore_CoreSettings>();
	if (Settings)
	{
		Settings->bEnableEventSystemLogging = (NewState == ECheckBoxState::Checked);
		Settings->TryUpdateDefaultConfigFile();
		UE_LOG(LogModulusEditor, Log, TEXT("Event System Logging %s"),
			Settings->bEnableEventSystemLogging ? TEXT("enabled") : TEXT("disabled"));
	}
#endif
}

void SModulusHubTab::OnUIDebugOverlayCheckStateChanged(ECheckBoxState NewState)
{
#if WITH_EDITORONLY_DATA
	UMCore_CoreSettings* Settings = GetMutableDefault<UMCore_CoreSettings>();
	if (Settings)
	{
		Settings->bShowUILayerDebugOverlay = (NewState == ECheckBoxState::Checked);
		Settings->TryUpdateDefaultConfigFile();
		UE_LOG(LogModulusEditor, Log, TEXT("UI Layer Debug Overlay %s"),
			Settings->bShowUILayerDebugOverlay ? TEXT("enabled") : TEXT("disabled"));
	}
#endif
}

ECheckBoxState SModulusHubTab::GetEventLoggingCheckState() const
{
#if WITH_EDITORONLY_DATA
	const UMCore_CoreSettings* Settings = UMCore_CoreSettings::Get();
	if (Settings)
	{
		return Settings->bEnableEventSystemLogging ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}
#endif
	return ECheckBoxState::Unchecked;
}

ECheckBoxState SModulusHubTab::GetUIDebugOverlayCheckState() const
{
#if WITH_EDITORONLY_DATA
	const UMCore_CoreSettings* Settings = UMCore_CoreSettings::Get();
	if (Settings)
	{
		return Settings->bShowUILayerDebugOverlay ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}
#endif
	return ECheckBoxState::Unchecked;
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
	// TODO: Add actual documentation URL when available
	UE_LOG(LogModulusEditor, Log, TEXT("[Docs] Quick Start Guide requested"));
	return FReply::Handled();
}

FReply SModulusHubTab::OnAPIReferenceClicked()
{
	// TODO: Add actual documentation URL when available
	UE_LOG(LogModulusEditor, Log, TEXT("[Docs] API Reference requested"));
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
