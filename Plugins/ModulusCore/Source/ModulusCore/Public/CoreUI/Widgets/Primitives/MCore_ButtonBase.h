// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_ButtonBase.h
 *
 * Base button class with theme system integration and optional text/icon bindings.
 * Automatically binds to UISubsystem::OnThemeChanged for runtime theme switching.
 */

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "MCore_ButtonBase.generated.h"

class UMCore_PDA_UITheme_Base;
class UCommonButtonStyle;
class UCommonTextStyle;
class UWidgetSwitcher;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModulusButtonClicked);

UENUM(BlueprintType)
enum class EMCore_ButtonDisplayMode : uint8
{
	TextOnly,
	IconOnly,
	TextAndIcon
};

/**
 * Base button class with theme system integration.
 *
 * Key Features:
 * - Automatic theme binding via UISubsystem::OnThemeChanged
 * - Per-instance style overrides that take precedence over theme defaults
 * - Text/icon display mode switching
 *
 * Optional child widget bindings (BindWidgetOptional):
 * - Txt_BtnLabel: Button text display
 * - Img_BtnIcon: Button icon display
 *
 * Blueprint Usage:
 * 1. Create Blueprint extending this class
 * 2. Add child widgets matching bind names (optional)
 * 3. Override ApplyTheme in C++ or handle in Blueprint via OnThemeApplied
 */
UCLASS(Abstract, Blueprintable, meta=(DisableNativeTick))
class MODULUSCORE_API UMCore_ButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UMCore_ButtonBase();

	// ============================================================================
	// TEXT API
	// ============================================================================

	/**
	 * Set button label text.
	 *
	 * @param InText - Text to display on the button
	 */
	UFUNCTION(BlueprintCallable, Category = "Button|Text")
	void SetButtonText(const FText& InText);

	UFUNCTION(BlueprintPure, Category = "Button|Text")
	FText GetButtonText() const;

	UFUNCTION(BlueprintPure, Category = "Button|Text")
	UCommonTextBlock* GetTextBlock() const { return Txt_BtnLabel; }

	// ============================================================================
	// STYLE API
	// ============================================================================

	/**
	 * Set button style override (takes precedence over theme).
	 * Pass nullptr to revert to theme default.
	 *
	 * @param InStyle - Button style class to apply, or nullptr for theme default
	 */
	UFUNCTION(BlueprintCallable, Category = "Button|Style")
	void SetButtonStyleOverride(TSubclassOf<UCommonButtonStyle> InStyle);

	UFUNCTION(BlueprintPure, Category = "Button|Style")
	TSubclassOf<UCommonButtonStyle> GetButtonStyleOverride() const { return ButtonStyleOverride; }

	/**
	 * Set text style override (takes precedence over theme).
	 *
	 * @param InStyle - Text style class to apply, or nullptr for theme default
	 */
	UFUNCTION(BlueprintCallable, Category = "Button|Style")
	void SetTextStyleOverride(TSubclassOf<UCommonTextStyle> InStyle);

	UFUNCTION(BlueprintPure, Category = "Button|Style")
	TSubclassOf<UCommonTextStyle> GetTextStyleOverride() const { return TextStyleOverride; }

	UFUNCTION(BlueprintPure, Category = "Button|Style")
	bool IsUsingStyleOverrides() const { return ButtonStyleOverride != nullptr || TextStyleOverride != nullptr; }

	// ============================================================================
	// ICON AND DISPLAY
	// ============================================================================

	/**
	 * Set button icon from a loaded texture.
	 *
	 * @param InIcon - Texture to display, or nullptr to hide
	 */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonIcon(UTexture2D* InIcon);

	/**
	 * Set button icon using soft reference (loads synchronously).
	 *
	 * @param InIcon - Soft reference to icon texture
	 */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonIconSoft(TSoftObjectPtr<UTexture2D> InIcon);

	/**
	 * Set button display mode (text only, icon only, or both).
	 *
	 * @param InMode - Display mode to apply
	 */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetDisplayMode(EMCore_ButtonDisplayMode InMode);

	UFUNCTION(BlueprintPure, Category = "Button")
	EMCore_ButtonDisplayMode GetDisplayMode() const { return DisplayMode; }

	// ============================================================================
	// WIDGET BINDINGS AND DELEGATES
	// ============================================================================

	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> Txt_BtnLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidgetOptional))
	TObjectPtr<UImage> Img_BtnIcon;

	UPROPERTY(BlueprintAssignable, Category = "Button")
	FOnModulusButtonClicked OnButtonClicked;

	/**
	 * Programmatically trigger this button's click logic (sound, animation, delegates).
	 */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SimulateClick();

protected:
	// ============================================================================
	// LIFECYCLE
	// ============================================================================

	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeOnClicked() override;
	virtual void NativeDestruct() override;

	// ============================================================================
	// THEME
	// ============================================================================

	/**
	 * Apply theme to button. Called on init and when theme change occurs.
	 * Override in derived classes for custom theme handling.
	 *
	 * @param Theme - Active theme, may be nullptr if no theme configured
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Theme")
	void ApplyTheme(UMCore_PDA_UITheme_Base* Theme);
	virtual void ApplyTheme_Implementation(UMCore_PDA_UITheme_Base* Theme);

	UFUNCTION(BlueprintImplementableEvent, Category = "Theme", meta = (DisplayName = "On Theme Applied"))
	void K2_OnThemeApplied(UMCore_PDA_UITheme_Base* Theme);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button")
	EMCore_ButtonDisplayMode DisplayMode = EMCore_ButtonDisplayMode::TextOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button|Text", meta = (MultiLine = false))
	FText ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button|Icon")
	TObjectPtr<UTexture2D> ButtonIcon;

	/* Override takes precedence over theme's PrimaryButtonStyle. Leave None for theme default. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button|Style")
	TSubclassOf<UCommonButtonStyle> ButtonStyleOverride;

	/* Override takes precedence over theme's BodyTextStyle. Leave None for theme default. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button|Style")
	TSubclassOf<UCommonTextStyle> TextStyleOverride;

private:
	void SyncPropertiesToWidgets();

	UFUNCTION()
	void HandleThemeChanged(UMCore_PDA_UITheme_Base* NewTheme);

	void BindThemeDelegate();
	void UnbindThemeDelegate();

	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UMCore_PDA_UITheme_Base> CachedTheme;

	bool bThemeDelegateBound{false};
};
