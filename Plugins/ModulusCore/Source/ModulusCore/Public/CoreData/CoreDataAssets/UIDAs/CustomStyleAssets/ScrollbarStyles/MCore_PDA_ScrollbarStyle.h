// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MCore_PDA_ScrollbarStyle.generated.h"

/**
 * Scrollbar styling for UI themes.
 * Referenced by UMCore_PDA_UITheme_Base.
 */
UCLASS(BlueprintType, Blueprintable)
class MODULUSCORE_API UMCore_PDA_ScrollbarStyle : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Scrollbar track/background image */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scrollbar")
	TObjectPtr<UObject> TrackImage;

	/** Scrollbar thumb/handle image */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scrollbar")
	TObjectPtr<UObject> ThumbImage;

	/** Track width */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scrollbar")
	float TrackWidth{ 12.0f };

	/** Track tint */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scrollbar|Colors")
	FSlateColor TrackTint = FLinearColor{ 0.1f, 0.1f, 0.1f, 0.5f };

	/** Thumb tint - normal */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scrollbar|Colors")
	FSlateColor ThumbNormalTint = FLinearColor{ 0.4f, 0.4f, 0.4f, 1.0f };

	/** Thumb tint - hovered */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scrollbar|Colors")
	FSlateColor ThumbHoveredTint = FLinearColor{ 0.6f, 0.6f, 0.6f, 1.0f };

	/** Thumb tint - dragged */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scrollbar|Colors")
	FSlateColor ThumbDraggedTint = FLinearColor{ 0.8f, 0.8f, 0.8f, 1.0f };

	/** Minimum thumb length */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scrollbar")
	float MinThumbLength{ 32.0f };

	/** Padding around scrollbar */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scrollbar")
	FMargin Padding{ 2.0f };

};