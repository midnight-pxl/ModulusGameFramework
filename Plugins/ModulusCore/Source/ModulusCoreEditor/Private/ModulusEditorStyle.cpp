// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "ModulusEditorStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr<FSlateStyleSet> FModulusEditorStyle::StyleInstance = nullptr;

/** Icon Names */
const FName FModulusEditorStyle::ModulusIconName = TEXT("ModulusEditor.ModulusIcon");
const FName FModulusEditorStyle::SettingsIconName = TEXT("ModulusEditor.SettingsIcon");
const FName FModulusEditorStyle::EcosystemIconName = TEXT("ModulusEditor.EcosystemIcon");
const FName FModulusEditorStyle::DocsIconName = TEXT("ModulusEditor.DocsIcon");

/** Brand Colors - Modulus Game Framework themed */
namespace ModulusColors
{
	/** Background colors */
	const FLinearColor Background = FLinearColor(0.110f, 0.110f, 0.12f, 1.0f);
	const FLinearColor SectionBackground = FLinearColor(0.145f, 0.145f, 0.15f, 1.0f);
	const FLinearColor HeaderBackground = FLinearColor(0.227f, 0.227f, 0.235f, 1.0f);
	
	/** Modulus Primary/Secondary colors */
	const FLinearColor Accent = FLinearColor(1.0f, 0.420f, 0.1f, 1.0f);
	const FLinearColor Secondary = FLinearColor(0.0f, 0.75f, 1.0f, 1.0f);
	
	/** Status colors */
	const FLinearColor Success = FLinearColor(0.2f, 0.9f, 0.4f, 1.0f);
	const FLinearColor Warning = FLinearColor(1.0f, 0.8f, 0.2f, 1.0f);
	const FLinearColor Error = FLinearColor(1.0f, 0.3f, 0.3f, 1.0f);

	/** Interactive state colors */
	const FLinearColor Disabled = FLinearColor(0.05f, 0.05f, 0.05f, 1.0f);

	/** Text colors */
	const FLinearColor TextPrimary = FLinearColor(0.91f, 0.91f, 0.91f, 1.0f);
	const FLinearColor TextSecondary = FLinearColor(0.65f, 0.65f, 0.65f, 1.0f);
	const FLinearColor TextMuted = FLinearColor(0.43f, 0.43f, 0.43f, 1.0f);
}

void FModulusEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FModulusEditorStyle::Shutdown()
{
	if (StyleInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
		StyleInstance.Reset();
	}
}

void FModulusEditorStyle::ReloadTextures()
{
	if (StyleInstance.IsValid())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

FName FModulusEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ModulusEditorStyle"));
	return StyleSetName;
}

FLinearColor FModulusEditorStyle::GetAccentColor() { return ModulusColors::Accent; }

FLinearColor FModulusEditorStyle::GetSecondaryColor() { return ModulusColors::Secondary; }

FLinearColor FModulusEditorStyle::GetBackgroundColor() { return ModulusColors::Background; }

FLinearColor FModulusEditorStyle::GetHeaderBackgroundColor(){ return ModulusColors::HeaderBackground; }

FLinearColor FModulusEditorStyle::GetSuccessColor() { return ModulusColors::Success; }

FLinearColor FModulusEditorStyle::GetWarningColor() { return ModulusColors::Warning; }

FLinearColor FModulusEditorStyle::GetErrorColor() {	return ModulusColors::Error; }

TSharedRef<FSlateStyleSet> FModulusEditorStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("ModulusCore"))->GetBaseDir() / TEXT("Resources"));

	/** Define color brushes */
	Style->Set("ModulusEditor.Background", new FSlateColorBrush(ModulusColors::Background));
	Style->Set("ModulusEditor.HeaderBackground", new FSlateColorBrush(ModulusColors::HeaderBackground));
	Style->Set("ModulusEditor.SectionBackground", new FSlateColorBrush(ModulusColors::SectionBackground));
	Style->Set("ModulusEditor.AccentBrush", new FSlateColorBrush(ModulusColors::Accent));
	Style->Set("ModulusEditor.SecondaryBrush", new FSlateColorBrush(ModulusColors::Secondary));

	/** Rounded border brush for sections */
	Style->Set("ModulusEditor.RoundedBox", new FSlateRoundedBoxBrush(
		ModulusColors::SectionBackground,
		4.0f
	));

	Style->Set("ModulusEditor.HeaderBox", new FSlateRoundedBoxBrush(
		ModulusColors::HeaderBackground,
		6.0f
	));

	Style->Set("ModulusEditor.AccentBox", new FSlateRoundedBoxBrush(
		FLinearColor(ModulusColors::Accent.R, ModulusColors::Accent.G, ModulusColors::Accent.B, 0.15f),
		4.0f
	));

	/** Separator brush */
	Style->Set("ModulusEditor.Separator", new FSlateColorBrush(FLinearColor(1.0f, 1.0f, 1.0f, 0.1f)));

	/** Text styles */
	const FTextBlockStyle NormalText = FTextBlockStyle()
		.SetFont(FCoreStyle::GetDefaultFontStyle("Regular", 10))
		.SetColorAndOpacity(FSlateColor(ModulusColors::TextPrimary));

	const FTextBlockStyle HeaderText = FTextBlockStyle()
		.SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 18))
		.SetColorAndOpacity(FSlateColor(ModulusColors::TextPrimary));

	const FTextBlockStyle SectionHeaderText = FTextBlockStyle()
		.SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 14))
		.SetColorAndOpacity(FSlateColor(ModulusColors::Secondary));

	const FTextBlockStyle SubHeaderText = FTextBlockStyle()
		.SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 12))
		.SetColorAndOpacity(FSlateColor(ModulusColors::TextPrimary));

	const FTextBlockStyle MutedText = FTextBlockStyle()
		.SetFont(FCoreStyle::GetDefaultFontStyle("Regular", 9))
		.SetColorAndOpacity(FSlateColor(ModulusColors::TextMuted));

	Style->Set("ModulusEditor.Text.Normal", NormalText);
	Style->Set("ModulusEditor.Text.Header", HeaderText);
	Style->Set("ModulusEditor.Text.SectionHeader", SectionHeaderText);
	Style->Set("ModulusEditor.Text.SubHeader", SubHeaderText);
	Style->Set("ModulusEditor.Text.Muted", MutedText);

	/** Button style */
	const FButtonStyle ModulusButton = FButtonStyle()
		.SetNormal(FSlateRoundedBoxBrush(ModulusColors::SectionBackground, 4.0f))
		.SetHovered(FSlateRoundedBoxBrush(ModulusColors::Accent * 0.8f, 4.0f))
		.SetPressed(FSlateRoundedBoxBrush(ModulusColors::Accent, 4.0f))
		.SetDisabled(FSlateRoundedBoxBrush(ModulusColors::Disabled, 4.0f))
		.SetNormalForeground(FSlateColor(ModulusColors::TextPrimary))
		.SetHoveredForeground(FSlateColor(ModulusColors::TextPrimary))
		.SetPressedForeground(FSlateColor(ModulusColors::TextPrimary))
		.SetDisabledForeground(FSlateColor(ModulusColors::TextMuted))
		.SetNormalPadding(FMargin(12.0f, 6.0f))
		.SetPressedPadding(FMargin(12.0f, 7.0f, 12.0f, 5.0f));

	Style->Set("ModulusEditor.Button", ModulusButton);

	/** Primary action button (accent colored) */
	const FButtonStyle PrimaryButton = FButtonStyle()
		.SetNormal(FSlateRoundedBoxBrush(ModulusColors::Accent * 0.9f, 4.0f))
		.SetHovered(FSlateRoundedBoxBrush(ModulusColors::Accent, 4.0f))
		.SetPressed(FSlateRoundedBoxBrush(ModulusColors::Accent * 1.1f, 4.0f))
		.SetDisabled(FSlateRoundedBoxBrush(ModulusColors::Disabled, 4.0f))
		.SetNormalForeground(FSlateColor(ModulusColors::TextPrimary))
		.SetHoveredForeground(FSlateColor(ModulusColors::TextPrimary))
		.SetPressedForeground(FSlateColor(ModulusColors::TextPrimary))
		.SetDisabledForeground(FSlateColor(ModulusColors::TextMuted))
		.SetNormalPadding(FMargin(12.0f, 6.0f))
		.SetPressedPadding(FMargin(12.0f, 7.0f, 12.0f, 5.0f));

	Style->Set("ModulusEditor.Button.Primary", PrimaryButton);

	/** Register icons - using colored boxes as placeholders until image assets are created */
	
	/** Main Modulus icon - used in toolbar */
	Style->Set(ModulusIconName, new FSlateRoundedBoxBrush(ModulusColors::Accent, 4.0f, FVector2f(16.0f, 16.0f)));
	Style->Set("ModulusEditor.ModulusIcon.Small", new FSlateRoundedBoxBrush(ModulusColors::Accent, 2.0f, FVector2f(16.0f, 16.0f)));
	Style->Set("ModulusEditor.ModulusIcon.Large", new FSlateRoundedBoxBrush(ModulusColors::Accent, 6.0f, FVector2f(40.0f, 40.0f)));

	/** Section icons - using colored boxes as placeholders */
	Style->Set(SettingsIconName, new FSlateRoundedBoxBrush(ModulusColors::HeaderBackground, 2.0f, FVector2f(16.0f, 16.0f)));
	Style->Set(EcosystemIconName, new FSlateRoundedBoxBrush(ModulusColors::TextPrimary, 2.0f, FVector2f(16.0f, 16.0f)));  // Monochrome logo
	Style->Set(DocsIconName, new FSlateRoundedBoxBrush(ModulusColors::Secondary, 2.0f, FVector2f(16.0f, 16.0f)));

	return Style;
}
