// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

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

UENUM(BlueprintType)
enum class EMCore_TextJustify : uint8
{
	Left,
	Center,
	Right
};

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
 * Automatically binds to UISubsystem::OnThemeChanged for runtime theme switching.
 * Derived Blueprint widgets override visuals; ApplyTheme() handles style updates.
 *
 * Optional child widget bindings (BindWidgetOptional):
 * - Text_ButtonLabel: Button text display
 * - Image_ButtonIcon: Button icon display
 *
 * Usage:
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

	/** Set button label text */
	UFUNCTION(BlueprintCallable, Category = "Button|Text")
	void SetButtonText(const FText& InText);

	/** Get current button label text */
	UFUNCTION(BlueprintPure, Category = "Button|Text")
	FText GetButtonText() const;

	/** Set text justification/alignment */
	UFUNCTION(BlueprintCallable, Category = "Button|Text")
	void SetTextJustification(EMCore_TextJustify InJustification);

	/** Get current text justification */
	UFUNCTION(BlueprintPure, Category = "Button|Text")
	EMCore_TextJustify GetTextJustification() const { return TextJustification; }

	/** Get the underlying text block for advanced customization. May return nullptr if not bound. */
	UFUNCTION(BlueprintPure, Category = "Button|Text")
	UCommonTextBlock* GetTextBlock() const { return Txt_BtnLabel; }

	/** Set button style override (takes precedence over theme). Pass nullptr to use theme default. */
	UFUNCTION(BlueprintCallable, Category = "Button|Style")
	void SetButtonStyleOverride(TSubclassOf<UCommonButtonStyle> InStyle);

	/** Get current button style override (nullptr if using theme default) */
	UFUNCTION(BlueprintPure, Category = "Button|Style")
	TSubclassOf<UCommonButtonStyle> GetButtonStyleOverride() const { return ButtonStyleOverride; }

	/** Set text style override (takes precedence over theme). Pass nullptr to use theme default. */
	UFUNCTION(BlueprintCallable, Category = "Button|Style")
	void SetTextStyleOverride(TSubclassOf<UCommonTextStyle> InStyle);

	/** Get current text style override (nullptr if using theme default) */
	UFUNCTION(BlueprintPure, Category = "Button|Style")
	TSubclassOf<UCommonTextStyle> GetTextStyleOverride() const { return TextStyleOverride; }

	/** Returns true if this button is using local style overrides instead of theme defaults */
	UFUNCTION(BlueprintPure, Category = "Button|Style")
	bool IsUsingStyleOverrides() const { return ButtonStyleOverride != nullptr || TextStyleOverride != nullptr; }

	/** Set button icon */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonIcon(UTexture2D* InIcon);

	/** Set button icon from soft reference (async-friendly) */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonIconSoft(TSoftObjectPtr<UTexture2D> InIcon);

	/** Set button display mode (text only, icon only, or both) */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetDisplayMode(EMCore_ButtonDisplayMode InMode);

	/** Get current button display mode */
	UFUNCTION(BlueprintPure, Category = "Button")
	EMCore_ButtonDisplayMode GetDisplayMode() const { return DisplayMode; }

	//~ Begin Optional Widget Bindings
	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> Txt_BtnLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidgetOptional))
	TObjectPtr<UImage> Img_BtnIcon;
	
	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> Switcher_Content;

protected:
	//~ UUserWidget Interface
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	//~ End UUserWidget Interface

	/**
	 * Apply theme to button. Called on init and when theme change occurs.
	 * Override in derived classes for custom theme handling.
	 *
	 * @param Theme - Active theme, may be nullptr if no theme configured
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Theme")
	void ApplyTheme(UMCore_PDA_UITheme_Base* Theme);
	virtual void ApplyTheme_Implementation(UMCore_PDA_UITheme_Base* Theme);

	/** Blueprint event when theme is applied (alternative to override) */
	UFUNCTION(BlueprintImplementableEvent, Category = "Theme", meta = (DisplayName = "On Theme Applied"))
	void K2_OnThemeApplied(UMCore_PDA_UITheme_Base* Theme);

	/** Button display mode (text, icon, or both) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button")
	EMCore_ButtonDisplayMode DisplayMode = EMCore_ButtonDisplayMode::TextOnly;

	/** Button label text - editable at design-time and runtime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button|Text", meta = (MultiLine = false))
	FText ButtonText;

	/** Text alignment/justification */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button|Text")
	EMCore_TextJustify TextJustification = EMCore_TextJustify::Center;

	/**
	 * Optional button style override. When set, takes precedence over the active theme's button style.
	 * Leave empty/None to use the theme's PrimaryButtonStyle.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button|Style")
	TSubclassOf<UCommonButtonStyle> ButtonStyleOverride;

	/**
	 * Optional text style override. When set, takes precedence over the active theme's text style.
	 * Leave empty/None to use the theme's BodyTextStyle.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button|Style")
	TSubclassOf<UCommonTextStyle> TextStyleOverride;

private:
	/** Sync design-time properties to bound child widgets */
	void SyncPropertiesToWidgets();

	/** Callback for UISubsystem theme change delegate */
	UFUNCTION()
	void HandleThemeChanged(UMCore_PDA_UITheme_Base* NewTheme);

	/** Subscribe to theme change notifications */
	void BindThemeDelegate();

	/** Unsubscribe from theme change notifications */
	void UnbindThemeDelegate();

	/** Tracks whether theme delegate is currently bound */
	bool bThemeDelegateBound{false};
};
