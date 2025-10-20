// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "MCore_ButtonBase.generated.h"

class UCommonTextBlock;
class UImage;

/**
 * ModulusCore button base with integrated text and icon widgets
 * 
 * Provides consistent API for setting button content:
 * - SetButtonText() - Show/hide text label
 * - SetButtonIcon() - Show/hide icon
 * 
 * Blueprint Setup:
 * - Add CommonTextBlock named "Text_Label" (optional binding)
 * - Add Image named "Image_Icon" (optional binding)
 * 
 * If widgets are missing, functions gracefully no-op.
 */
UCLASS(Abstract, Blueprintable, ClassGroup="ModulusUI", meta=(DisableNativeTick))
class MODULUSCORE_API UMCore_ButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UMCore_ButtonBase(const FObjectInitializer& ObjectInitializer);

	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface

	/**
	 * Get current button text
	 */
	UFUNCTION(BlueprintPure, Category = "Button Content")
	FText GetButtonText() const;
	
	/**
	 * Set button text (shows text widget if hidden)
	 * @param InText - Text to display (empty text hides widget)
	 */
	UFUNCTION(BlueprintCallable, Category = "Button Content")
	void SetButtonText(const FText& InText);

	/**
	 * Set button icon (shows icon widget if hidden)
	 * @param InIcon - Texture to display (nullptr hides widget)
	 */
	UFUNCTION(BlueprintCallable, Category = "Button Content")
	void SetButtonIcon(UTexture2D* InIcon);

	/**
	 * Enable/Disable button's interactivity (greyed out when disabled)
	 */
	UFUNCTION(BlueprintCallable, Category = "Button Content")
	void SetButtonEnabled(bool bInEnabled);

protected:
	//~ Start UCommonButtonBase Interface
	virtual void NativeOnClicked() override;
	//~ End UCommonButtonBase Interface

	/** Text label widget (optional - gracefully handles if missing) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> Text_ButtonTxt;

	/** Icon image widget (optional - gracefully handles if missing) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_ButtonIcon;

private:
	/** Apply theme set in Modulus Developer Settings */
	void ApplyUITheme();

	/** To avoid repeated lookups */
	bool bHasThemeApplied{false};
};

