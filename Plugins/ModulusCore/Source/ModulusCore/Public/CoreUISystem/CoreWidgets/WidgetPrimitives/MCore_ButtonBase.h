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
UCLASS(Abstract, BlueprintType, Blueprintable)
class MODULUSCORE_API UMCore_ButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:

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
	 * Get current button text
	 */
	UFUNCTION(BlueprintPure, Category = "Button Content")
	FText GetButtonText() const;

protected:
	/** Text label widget (optional - gracefully handles if missing) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> Text_Label;

	/** Icon image widget (optional - gracefully handles if missing) */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_Icon;
};

