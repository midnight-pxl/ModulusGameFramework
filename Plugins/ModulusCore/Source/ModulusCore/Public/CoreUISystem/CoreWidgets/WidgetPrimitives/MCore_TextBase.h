// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "MCore_TextBase.generated.h"

/**
 * Base text block with automatic theme styling.
 * 
 * Key Features:
 * - Auto-applies theme text styles (Heading/Body/Label)
 * - Helper for dynamic color changes
 */
UCLASS(Abstract, Blueprintable, ClassGroup="ModulusUI", meta=(DisableNativeTick))
class MODULUSCORE_API UMCore_TextBase : public UCommonTextBlock
{
	GENERATED_BODY()

public:
	UMCore_TextBase(const FObjectInitializer& ObjectInitializer);

	//~ Start UUserWidget Interface
	virtual void NativePreConstruct();
	//~ End UUserWidget Interface

	/** Set text color and/or opacity without changing style */
	UFUNCTION(BlueprintCallable, Category="Text Content")
	void SetTextColorOpacity(FLinearColor Color);

protected:
	/** Apply theme set in Modulus Developer Settings */
	void ApplyThemeStyle();

	/** To avoid repeated lookups */
	bool bHasThemeApplied{false};
	
};
