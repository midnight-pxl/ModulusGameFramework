// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

/**
 * Modulus Editor Style Set
 * Provides consistent styling for all Modulus editor UI elements.
 */
class FModulusEditorStyle
{
public:
	static void Initialize();
	static void Shutdown();
	static void ReloadTextures();

	static const ISlateStyle& Get() { return *StyleInstance; }
	static FName GetStyleSetName();

	// Icon names
	static const FName ModulusIconName;
	static const FName SettingsIconName;
	static const FName EcosystemIconName;
	static const FName DocsIconName;
	static const FName RefreshIconName;
	static const FName ValidateIconName;
	static const FName ExportIconName;
	static const FName ImportIconName;
	static const FName ResetIconName;

	// Color accessors
	static FLinearColor GetAccentColor();
	static FLinearColor GetSecondaryColor();
	static FLinearColor GetBackgroundColor();
	static FLinearColor GetHeaderBackgroundColor();
	static FLinearColor GetSuccessColor();
	static FLinearColor GetWarningColor();
	static FLinearColor GetErrorColor();

private:
	static TSharedRef<FSlateStyleSet> Create();
	static TSharedPtr<FSlateStyleSet> StyleInstance;
};
