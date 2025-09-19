// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "MCore_GraphicsSettingsData.generated.h"

enum class EMCore_GraphicsPreset : uint8;

USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_GraphicsSettingsData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Display")
	TArray<FIntPoint> SupportedResolutions;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Display")
	TArray<TEnumAsByte<EWindowMode::Type>> SupportedWindowModes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Display")
	TArray<FText> VSyncOptions; // Off, On, Adaptive
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Performance")
	TArray<int32> FrameRateLimits; // 30, 60, 120, 144, Unlimited
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quality")
	TArray<EMCore_GraphicsPreset> QualityPresets;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quality")
	TArray<FText> AntiAliasingOptions; // Off, FXAA, TAA, MSAA 2x/4x/8x
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quality")
	TArray<FText> TextureQualityOptions; // Low, Medium, High, Ultra
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quality")
	TArray<FText> ShadowQualityOptions;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quality")
	TArray<FText> EffectsQualityOptions;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quality")
	TArray<FText> ViewDistanceOptions;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Display")
	TArray<FString> AvailableDisplayAdapters;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ray Tracing")
	TArray<FText> RayTracingOptions; // Off, Low, Medium, High, Ultra
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upscaling")
	TArray<FText> UpscalingOptions; // Off, Quality, Balanced, Performance, Ultra Performance
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reflections")
	TArray<FText> ReflectionOptions; // Off, SSR Low, SSR High, RT Reflections
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lighting")
	TArray<FText> AmbientOcclusionOptions; // Off, SSAO, HBAO+, RT AO
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Environment")
	TArray<FText> FoliageQualityOptions; // Low, Medium, High, Ultra
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Post Processing")
	bool bSupportsMotionBlur{true};
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Post Processing")
	bool bSupportsDepthOfField{true};
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Post Processing")
	bool bSupportsBloom{true};
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Display")
	bool bSupportsHDR{false};
};