// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "MCore_ButtonBase.generated.h"

class UMCore_PDA_UITheme_Base;
class UCommonTextBlock;
class UWidgetSwitcher;
class UImage;

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
UCLASS(Abstract, Blueprintable)
class MODULUSCORE_API UMCore_ButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	UMCore_ButtonBase();

	/** Set button label text */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonText(const FText& InText);

	/** Get current button label text */
	UFUNCTION(BlueprintPure, Category = "Button")
	FText GetButtonText() const;

	/** Set button icon */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonIcon(UTexture2D* InIcon);

	/** Set button icon from soft reference (async-friendly) */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonIconSoft(TSoftObjectPtr<UTexture2D> InIcon);
	
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetDisplayMode(EMCore_ButtonDisplayMode InMode);

	UFUNCTION(BlueprintPure, Category = "Button")
	EMCore_ButtonDisplayMode GetDisplayMode() const { return DisplayMode; }

protected:
	//~ UUserWidget Interface
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

	//~ Optional child widget bindings
	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> Txt_BtnLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidgetOptional))
	TObjectPtr<UImage> Img_BtnIcon;
	
	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> Switcher_Content;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button")
	EMCore_ButtonDisplayMode DisplayMode = EMCore_ButtonDisplayMode::TextOnly;

private:
	UFUNCTION()
	void HandleThemeChanged(UMCore_PDA_UITheme_Base* NewTheme);

	void BindThemeDelegate();
	void UnbindThemeDelegate();

	bool bThemeDelegateBound{false};
};
