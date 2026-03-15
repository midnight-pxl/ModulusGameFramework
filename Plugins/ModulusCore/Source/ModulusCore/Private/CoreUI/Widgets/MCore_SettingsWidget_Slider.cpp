// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUI/Widgets/MCore_SettingsWidget_Slider.h"
#include "CoreData/Types/Settings/MCore_DA_SettingDefinition.h"
#include "CoreData/Types/Settings/MCore_SettingsTypes.h"
#include "CoreData/Libraries/MCore_GameSettingsLibrary.h"
#include "CoreData/Assets/UI/Themes/MCore_PDA_UITheme_Base.h"
#include "CoreData/Assets/UI/Styles/MCore_PDA_SliderStyle.h"
#include "CoreData/Logging/LogModulusUI.h"
#include "CoreUI/MCore_UISubsystem.h"
#include "CoreUI/Widgets/Primitives/MCore_ButtonBase.h"
#include "CommonTextBlock.h"
#include "Components/Slider.h"

// ============================================================================
// LIFECYCLE
// ============================================================================

void UMCore_SettingsWidget_Slider::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Slider_Value)
	{
		Slider_Value->OnValueChanged.AddDynamic(this, &ThisClass::HandleSliderValueChanged);
	}

	if (Btn_StepLeft && Btn_StepLeft->IsVisible())
	{
		Btn_StepLeft->OnButtonClicked.AddDynamic(this, &ThisClass::HandleStepLeft);
	}

	if (Btn_StepRight && Btn_StepRight->IsVisible())
	{
		Btn_StepRight->OnButtonClicked.AddDynamic(this, &ThisClass::HandleStepRight);
	}
}

// ============================================================================
// DEFINITION SET
// ============================================================================

void UMCore_SettingsWidget_Slider::OnDefinitionSet_Implementation(
	const UMCore_DA_SettingDefinition* Definition)
{
	if (!Definition || !Slider_Value)
	{
		return;
	}

	const float CurrentValue = UMCore_GameSettingsLibrary::GetSettingFloat(
		GetOwningLocalPlayer(), Definition);

	const float ClampedValue = FMath::Clamp(CurrentValue,
		Definition->MinValue, Definition->MaxValue);

	bIsUpdatingSlider = true;
	Slider_Value->SetMinValue(Definition->MinValue);
	Slider_Value->SetMaxValue(Definition->MaxValue);
	Slider_Value->SetStepSize(Definition->StepSize);
	Slider_Value->SetValue(ClampedValue);
	bIsUpdatingSlider = false;

	if (Txt_ValueDisplay)
	{
		Txt_ValueDisplay->SetText(FormatValue(ClampedValue));
	}
}

// ============================================================================
// SLIDER HANDLER
// ============================================================================

void UMCore_SettingsWidget_Slider::HandleSliderValueChanged(float RawValue)
{
	if (bIsUpdatingSlider)
	{
		return;
	}

	const float Snapped = SnapToStep(RawValue);

	if (!FMath::IsNearlyEqual(Snapped, RawValue, KINDA_SMALL_NUMBER))
	{
		bIsUpdatingSlider = true;
		Slider_Value->SetValue(Snapped);
		bIsUpdatingSlider = false;
	}

	ApplyValueToEngine(Snapped);
	SyncSliderAndDisplay(Snapped);
	BroadcastValueChanged();
}

// ============================================================================
// STEP BUTTONS
// ============================================================================

void UMCore_SettingsWidget_Slider::HandleStepLeft()
{
	StepLeft();
}

void UMCore_SettingsWidget_Slider::HandleStepRight()
{
	StepRight();
}

void UMCore_SettingsWidget_Slider::StepLeft_Implementation()
{
	if (!SettingDefinition)
	{
		return;
	}

	StepByAmount(-SettingDefinition->StepSize);
}

void UMCore_SettingsWidget_Slider::StepRight_Implementation()
{
	if (!SettingDefinition)
	{
		return;
	}

	StepByAmount(SettingDefinition->StepSize);
}

void UMCore_SettingsWidget_Slider::StepByAmount(float Delta)
{
	if (!SettingDefinition || !Slider_Value)
	{
		return;
	}

	const float Current = Slider_Value->GetValue();
	const float NewValue = FMath::Clamp(Current + Delta,
		SettingDefinition->MinValue, SettingDefinition->MaxValue);
	const float Snapped = SnapToStep(NewValue);

	bIsUpdatingSlider = true;
	Slider_Value->SetValue(Snapped);
	bIsUpdatingSlider = false;

	ApplyValueToEngine(Snapped);
	SyncSliderAndDisplay(Snapped);
	BroadcastValueChanged();
}

// ============================================================================
// HELPERS
// ============================================================================

float UMCore_SettingsWidget_Slider::SnapToStep(float RawValue) const
{
	if (!SettingDefinition || SettingDefinition->StepSize <= 0.f)
	{
		return RawValue;
	}

	const float Snapped = FMath::RoundToFloat(
		(RawValue - SettingDefinition->MinValue) / SettingDefinition->StepSize)
		* SettingDefinition->StepSize + SettingDefinition->MinValue;

	return FMath::Clamp(Snapped,
		SettingDefinition->MinValue, SettingDefinition->MaxValue);
}

FText UMCore_SettingsWidget_Slider::FormatValue(float Value) const
{
	if (!SettingDefinition)
	{
		return FText::GetEmpty();
	}

	switch (SettingDefinition->SliderDisplayFormat)
	{
	case EMCore_SliderDisplayFormat::Percentage:
		return FText::FromString(FString::Printf(TEXT("%d%%"),
			FMath::RoundToInt(Value * 100.f)));

	case EMCore_SliderDisplayFormat::WholeNumber:
		return FText::FromString(FString::Printf(TEXT("%d"),
			FMath::RoundToInt(Value)));

	case EMCore_SliderDisplayFormat::RawValue:
	default:
		return FText::FromString(FString::Printf(TEXT("%.2f"), Value));
	}
}

void UMCore_SettingsWidget_Slider::ApplyValueToEngine(float SnappedValue)
{
	if (!SettingDefinition)
	{
		return;
	}

	TArray<FMCore_FloatSettingChange> Changes;
	Changes.Add({const_cast<UMCore_DA_SettingDefinition*>(SettingDefinition.Get()), SnappedValue});
	UMCore_GameSettingsLibrary::SetSettingFloat(GetOwningLocalPlayer(), Changes, false);
}

void UMCore_SettingsWidget_Slider::SyncSliderAndDisplay(float SnappedValue)
{
	if (Txt_ValueDisplay)
	{
		Txt_ValueDisplay->SetText(FormatValue(SnappedValue));
	}
}

// ============================================================================
// VALUE INTERFACE OVERRIDES
// ============================================================================

FString UMCore_SettingsWidget_Slider::GetValueAsString_Implementation() const
{
	if (!Slider_Value)
	{
		return TEXT("0");
	}

	return FormatValue(Slider_Value->GetValue()).ToString();
}

void UMCore_SettingsWidget_Slider::ResetToDefault_Implementation()
{
	if (!SettingDefinition || !Slider_Value)
	{
		return;
	}

	const float DefaultVal = SettingDefinition->DefaultValue;

	bIsUpdatingSlider = true;
	Slider_Value->SetValue(DefaultVal);
	bIsUpdatingSlider = false;

	ApplyValueToEngine(DefaultVal);
	SyncSliderAndDisplay(DefaultVal);
}

// ============================================================================
// THEME
// ============================================================================

void UMCore_SettingsWidget_Slider::ApplyTheme_Implementation(UMCore_PDA_UITheme_Base* NewTheme)
{
	Super::ApplyTheme_Implementation(NewTheme);

	if (!NewTheme)
	{
		return;
	}

	int32 SizeIndex = 0;
	if (const ULocalPlayer* LP = GetOwningLocalPlayer())
	{
		if (const UMCore_UISubsystem* UI = LP->GetSubsystem<UMCore_UISubsystem>())
		{
			SizeIndex = UI->GetActiveTextSizeIndex();
		}
	}

	if (Txt_ValueDisplay && !NewTheme->CaptionTextStyle.IsEmpty())
	{
		const TSubclassOf<UCommonTextStyle> ResolvedCaptionStyle =
			NewTheme->CaptionTextStyle.IsValidIndex(SizeIndex)
			? NewTheme->CaptionTextStyle[SizeIndex]
			: NewTheme->CaptionTextStyle[0];

		if (ResolvedCaptionStyle)
		{
			Txt_ValueDisplay->SetStyle(ResolvedCaptionStyle);
		}
	}

	if (Slider_Value && NewTheme->SliderStyle)
	{
		const UMCore_PDA_SliderStyle* Style = NewTheme->SliderStyle;

		FSliderStyle SliderStyle = Slider_Value->GetWidgetStyle();

		// Bar brush
		FSlateBrush BarBrush;
		if (Style->BarImage)
		{
			BarBrush.SetResourceObject(Style->BarImage);
		}
		BarBrush.TintColor = Style->BarNormalTint;
		SliderStyle.SetNormalBarImage(BarBrush);

		// Thumb brush — normal
		FSlateBrush ThumbNormal;
		if (Style->ThumbImage)
		{
			ThumbNormal.SetResourceObject(Style->ThumbImage);
		}
		ThumbNormal.TintColor = Style->ThumbNormalTint;
		ThumbNormal.SetImageSize(Style->ThumbSize);
		SliderStyle.SetNormalThumbImage(ThumbNormal);

		// Thumb brush — hovered
		FSlateBrush ThumbHovered;
		if (Style->ThumbImage)
		{
			ThumbHovered.SetResourceObject(Style->ThumbImage);
		}
		ThumbHovered.TintColor = Style->ThumbHoveredTint;
		ThumbHovered.SetImageSize(Style->ThumbSize);
		SliderStyle.SetHoveredThumbImage(ThumbHovered);

		// Thumb brush — disabled (reuse normal)
		SliderStyle.SetDisabledThumbImage(ThumbNormal);

		Slider_Value->SetWidgetStyle(SliderStyle);
	}
}
