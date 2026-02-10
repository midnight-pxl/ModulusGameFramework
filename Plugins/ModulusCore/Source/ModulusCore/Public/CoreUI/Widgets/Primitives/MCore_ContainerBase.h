// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MCore_ContainerBase.generated.h"

class UMCore_PDA_UITheme_Base;
class UBorder;
class UImage;
class UNamedSlot;

/**
 * Base container widget with theme system integration.
 *
 * Use as themed background/border for UI panels and sections.
 * Automatically binds to UISubsystem::OnThemeChanged for runtime theme switching.
 *
 * Optional child widget bindings (BindWidgetOptional):
 * - Border_Background: Primary border for background color/brush
 * - Image_Background: Alternative image-based background
 * - Slot_Content: Named slot for child content
 *
 * Usage:
 * 1. Create Blueprint extending this class
 * 2. Add Border_Background or Image_Background for themed background
 * 3. Add Slot_Content as NamedSlot for child widgets
 * 4. Override ApplyTheme or handle via OnThemeApplied
 */
UCLASS(Abstract, Blueprintable, meta=(DisableNativeTick))
class MODULUSCORE_API UMCore_ContainerBase : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UMCore_ContainerBase();

	/** Set background color (applies to Border_Background) */
	UFUNCTION(BlueprintCallable, Category = "Container")
	void SetBackgroundColor(FLinearColor InColor);

	/** Set background brush (applies to Border_Background) */
	UFUNCTION(BlueprintCallable, Category = "Container")
	void SetBackgroundBrush(const FSlateBrush& InBrush);

	/** Set background image (applies to Image_Background) */
	UFUNCTION(BlueprintCallable, Category = "Container")
	void SetBackgroundImage(UTexture2D* InTexture);

	/** Set padding on Border_Background */
	UFUNCTION(BlueprintCallable, Category = "Container")
	void SetContentPadding(FMargin InPadding);

protected:
	//~ UUserWidget Interface
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	//~ End UUserWidget Interface

	/**
	 * Apply theme to this container. Called on init and when theme change occurs.
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

	//~ Begin Optional Widget Bindings
	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidgetOptional))
	TObjectPtr<UBorder> Border_Background;

	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_Background;

	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (BindWidgetOptional))
	TObjectPtr<UNamedSlot> Slot_Content;
	//~ End Optional Widget Bindings

private:
	/** Callback for UISubsystem theme change delegate */
	UFUNCTION()
	void HandleThemeChanged(UMCore_PDA_UITheme_Base* NewTheme);

	/** Subscribe to theme change notifications */
	void BindThemeDelegate();

	/** Unsubscribe from theme change notifications */
	void UnbindThemeDelegate();
	
	/** Cached theme reference */
	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UMCore_PDA_UITheme_Base> CachedTheme;

	/** Tracks whether theme delegate is currently bound */
	bool bThemeDelegateBound{false};
};
