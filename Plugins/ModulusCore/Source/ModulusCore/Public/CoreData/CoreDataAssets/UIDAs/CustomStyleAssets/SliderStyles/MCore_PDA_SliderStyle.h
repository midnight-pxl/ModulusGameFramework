// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MCore_PDA_SliderStyle.generated.h"

/**
 * Slider styling for UI themes.
 * Referenced by UMCore_PDA_UITheme_Base.
 */
UCLASS(BlueprintType, Blueprintable)
class MODULUSCORE_API UMCore_PDA_SliderStyle : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Slider bar background image */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider")
	TObjectPtr<UObject> BarImage;

	/** Slider thumb/handle image */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider")
	TObjectPtr<UObject> ThumbImage;

	/** Bar thickness */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider")
	float BarThickness{ 4.0f };

	/** Bar tint - normal */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider|Colors")
	FSlateColor BarNormalTint = FLinearColor{ 0.3f, 0.3f, 0.3f, 1.0f };

	/** Bar tint - filled portion */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider|Colors")
	FSlateColor BarFilledTint = FLinearColor{ 0.8f, 0.8f, 0.8f, 1.0f };

	/** Thumb tint - normal */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider|Colors")
	FSlateColor ThumbNormalTint = FLinearColor{ 0.5f, 0.5f, 0.5f, 1.0f };

	/** Thumb tint - hovered */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider|Colors")
	FSlateColor ThumbHoveredTint = FLinearColor{ 0.8f, 0.8f, 0.8f, 1.0f };

	/** Thumb tint - dragged */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider|Colors")
	FSlateColor ThumbDraggedTint = FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f };

	/** Thumb size */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slider")
	FVector2D ThumbSize{ 16.0, 16.0 };
	
};
