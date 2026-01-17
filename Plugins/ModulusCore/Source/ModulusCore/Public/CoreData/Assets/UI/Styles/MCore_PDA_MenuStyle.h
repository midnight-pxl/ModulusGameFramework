// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MCore_PDA_MenuStyle.generated.h"

class UTexture2D;
class UMaterialInterface;
class UUserWidget;
class UFont;

/**
 * Menu styling - colors, backgrounds, layout, branding.
 * Referenced by UMCore_PDA_UITheme_Base.
 */
UCLASS(BlueprintType, Blueprintable)
class MODULUSCORE_API UMCore_PDA_MenuStyle : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// =========================================================================
	// Colors
	// =========================================================================
	
	/** Primary brand color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Colors")
	FSlateColor ColorPrimary = FLinearColor{ 0.2f, 0.4f, 0.8f, 1.0f };

	/** Secondary color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Colors")
	FSlateColor ColorSecondary = FLinearColor{ 0.6f, 0.6f, 0.6f, 1.0f };

	/** Accent/highlight color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Colors")
	FSlateColor ColorAccent = FLinearColor{ 0.9f, 0.7f, 0.1f, 1.0f };

	/** Background color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Colors")
	FSlateColor ColorBackground = FLinearColor{ 0.02f, 0.02f, 0.02f, 0.9f };

	/** Primary text color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Colors")
	FSlateColor ColorText = FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f };

	/** Muted/disabled text color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Colors")
	FSlateColor ColorTextMuted = FLinearColor{ 0.5f, 0.5f, 0.5f, 1.0f };

	/** Header text color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Colors")
	FSlateColor ColorHeader = FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f };

	/** Error/destructive color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Colors")
	FSlateColor ColorError = FLinearColor{ 0.9f, 0.2f, 0.2f, 1.0f };

	/** Success color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Colors")
	FSlateColor ColorSuccess = FLinearColor{ 0.2f, 0.8f, 0.3f, 1.0f };

	/** Warning color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Colors")
	FSlateColor ColorWarning = FLinearColor{ 0.9f, 0.7f, 0.1f, 1.0f };

	// =========================================================================
	// Fonts
	// =========================================================================
	
	/** Main UI font family */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fonts")
	TObjectPtr<UFont> MainFontFamily;

	/** Header font family */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fonts")
	TObjectPtr<UFont> HeaderFontFamily;

	/** Main font scale */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fonts")
	float MainFontScale{ 1.0f };

	/** Header font scale */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fonts")
	float HeaderFontScale{ 1.0f };

	/** Credits font family */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fonts")
	TObjectPtr<UFont> CreditsFontFamily;

	/** Credits font color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fonts")
	FSlateColor CreditsFontColor = FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f };

	// =========================================================================
	// Backgrounds
	// =========================================================================
	
	/** Main menu background widget (for animated/complex backgrounds) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Backgrounds")
	TSubclassOf<UUserWidget> MainMenuBackgroundWidget;

	/** Pause menu background widget */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Backgrounds")
	TSubclassOf<UUserWidget> PauseMenuBackgroundWidget;

	/** Credits background widget */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Backgrounds")
	TSubclassOf<UUserWidget> CreditsBackgroundWidget;

	/** Loading screen background material */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Backgrounds")
	TObjectPtr<UMaterialInterface> LoadingScreenBackground;

	/** Start/welcome screen background material */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Backgrounds")
	TObjectPtr<UMaterialInterface> StartScreenBackground;

	/** Simple menu background image */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Backgrounds")
	TObjectPtr<UObject> MenuBackgroundImage;

	/** Menu background tint */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Backgrounds")
	FSlateColor MenuBackgroundTint = FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f };

	/** Menu background image margin (9-slice) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Backgrounds")
	FMargin MenuBackgroundImageMargin;

	/** Background blur strength */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Backgrounds", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float BackgroundBlurStrength{ 10.0f };

	// =========================================================================
	// Branding
	// =========================================================================
	
	/** Game logo */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Branding")
	TSoftObjectPtr<UTexture2D> LogoTexture;

	/** Logo size multiplier */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Branding")
	float LogoSize{ 1.0f };

	/** Start screen image */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Branding")
	TObjectPtr<UTexture2D> StartScreenImage;

	// =========================================================================
	// Headers & Separators
	// =========================================================================
	
	/** Header bar height */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Headers")
	float HeaderBarHeight{ 48.0f };

	/** Header separator image */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Headers")
	TObjectPtr<UObject> HeaderSeparatorImage;

	/** Header separator tint */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Headers")
	FSlateColor HeaderSeparatorTint = FLinearColor{ 1.0f, 1.0f, 1.0f, 0.3f };

	// =========================================================================
	// Layout & Spacing
	// =========================================================================
	
	/** Button internal padding */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Layout")
	FMargin ButtonPadding{ 24.0f, 12.0f, 24.0f, 12.0f };

	/** Menu button margin */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Layout")
	FMargin MenuButtonMargin{ 0.0f, 4.0f, 0.0f, 4.0f };

	/** Container/panel margin */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Layout")
	FMargin ContainerMargin{ 32.0f };

	/** Menu item spacing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Layout")
	float MenuItemSpacing{ 8.0f };

	/** Section spacing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Layout")
	float SectionSpacing{ 24.0f };

	// =========================================================================
	// Alignment
	// =========================================================================
	
	/** Menu vertical alignment */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Alignment")
	TEnumAsByte<EVerticalAlignment> MenuVerticalAlignment{ VAlign_Center };

	/** Menu button horizontal alignment */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Alignment")
	TEnumAsByte<EHorizontalAlignment> MenuButtonAlignment{ HAlign_Center };

	/** Default text justification */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Alignment")
	TEnumAsByte<ETextJustify::Type> DefaultTextJustification{ ETextJustify::Center };

	// =========================================================================
	// Loading Widget
	// =========================================================================
	
	/** Loading indicator material */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loading")
	TObjectPtr<UMaterialInterface> LoadingWidgetMaterial;

	/** Loading indicator color */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loading")
	FSlateColor LoadingWidgetColor = FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f };

	// =========================================================================
	// Tooltips
	// =========================================================================
	
	/** Tooltip additional width */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tooltips")
	int32 TooltipAdditionalWidth{ 0 };

	/** Tooltip text scale */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tooltips")
	float TooltipTextScale{ 1.0f };
	
};
