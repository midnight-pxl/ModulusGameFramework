// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreData/Types/Settings/MCore_DA_SettingDefinition.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

FString UMCore_DA_SettingDefinition::GetSaveKey() const
{
	if (!SettingTag.IsValid()) { return FString(); }
	/** GameplayTag Audio.SFXVolume becomes Audio_SFXVolume */
	return SettingTag.ToString().Replace(TEXT("."), TEXT("_"));
}

bool UMCore_DA_SettingDefinition::IsValid() const
{
	if (DisplayName.IsEmpty() || !SettingTag.IsValid()) { return false; }
	
	switch (SettingType)
	{
	case EMCore_SettingType::Slider:
		return MinValue <= MaxValue
		&& DefaultValue >= MinValue
		&& DefaultValue <= MaxValue;
		
	case EMCore_SettingType::Dropdown:
		return DropdownOptions.Num() > 0
		&& DefaultDropdownIndex >= 0
		&& DefaultDropdownIndex < DropdownOptions.Num();
		
	case EMCore_SettingType::KeyBinding:
		return InputAction != nullptr
		&& InputMappingContext != nullptr;
		
	case EMCore_SettingType::Toggle:
	default:
		return true;
	}
}

#if WITH_EDITOR
EDataValidationResult UMCore_DA_SettingDefinition::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	
	if (!SettingTag.IsValid())
	{
		Context.AddError(FText::FromString(
			FString::Printf(TEXT("%s: SettingTag not set. Setting definitions require a valid gameplay tag."),
				*GetName())));
		Result = EDataValidationResult::Invalid;
	}
	
	if (DisplayName.IsEmpty())
	{
		Context.AddError(FText::FromString(
			FString::Printf(TEXT("DisplayName: %s is empty"), *GetName())));
		Result = EDataValidationResult::Invalid;
	}
	
	switch (SettingType)
	{
	case EMCore_SettingType::Slider:
		if (MinValue > MaxValue)
		{
			Context.AddError(FText::FromString(
				FString::Printf(TEXT("%s: MinValue is greater than MaxValue"), *GetName())));
			Result = EDataValidationResult::Invalid;
		}
		if (DefaultValue < MinValue || DefaultValue > MaxValue)
		{
			Context.AddError(FText::FromString(
				FString::Printf(TEXT("%s: DefaultValue is out of range [%.2f, %.2f]"),
					*GetName(), MinValue, MaxValue)));
			Result = EDataValidationResult::Invalid;
		}
		break;
		
	case EMCore_SettingType::Dropdown:
		if (DropdownOptions.Num() == 0)
		{
			Context.AddError(FText::FromString(
				FString::Printf(TEXT("%s: Dropdown has no options"), *GetName())));
			Result = EDataValidationResult::Invalid;
		}
		else if (DefaultDropdownIndex < 0 || DefaultDropdownIndex >= DropdownOptions.Num())
		{
			Context.AddError(FText::FromString(
				FString::Printf(TEXT("%s: Dropdown index is out of range [0, %d]"),
					*GetName(), DropdownOptions.Num()-1)));
			Result = EDataValidationResult::Invalid;
		}
		break;
		
	case EMCore_SettingType::KeyBinding:
		if (InputAction == nullptr)
		{
			Context.AddError(FText::FromString(
				FString::Printf(TEXT("%s: KeyBinding requires an InputAction reference"), *GetName())));
			Result = EDataValidationResult::Invalid;
		}
		if (InputMappingContext == nullptr)
		{
			Context.AddError(FText::FromString(
				FString::Printf(TEXT("%s: KeyBinding requires an InputMappingContext reference"), *GetName())));
			Result = EDataValidationResult::Invalid;
		}
		break;
		
	default:
		break;
	}
	
	return Result;
}
#endif
