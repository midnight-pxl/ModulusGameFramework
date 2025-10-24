// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "MCore_SettingsWidget_Base.h"
#include "MCore_SettingsWidget_Toggle.generated.h"

UENUM()
enum class EMCore_ToggleFunctionCallers : uint8
{
	Getter		UMETA(DisplayName="Function to Get setting"),
	Setter		UMETA(DisplayName="Function to Set setting"),
	Query		UMETA(DisplayName="Query if setting enabled"),
	Dirty		UMETA(DisplayName="Query if setting is dirty (different than saved value)")
};

/**
 * Delegate for type-safe toggle value changes
 * @param SettingTag - Setting that changed
 * @param bNewValue - Settings new boolean value
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnToggleValueChanged,
	FGameplayTag, SettingTag,
	bool, bNewValue);

/**
 * Provides both generic (OnValueChanged) and type-safe (OnToggleValueChanged) delegates
 * Listen to type-safe delegate when you want the actual boolean value without string conversion 
 */
UCLASS()
class MODULUSCORE_API UMCore_SettingsWidget_Toggle : public UMCore_SettingsWidget_Base
{
	GENERATED_BODY()

public:
	
	/**
	 * Set toggle value (triggers events)
	 * Fires both OnValueChanged (generic) and OnToggleValueChanged (type-safe)
	 */
	UFUNCTION(BlueprintCallable, Category = "Settings Widget")
	void SetToggleValue(bool bNewValue);
    
	/** Get current toggle value */
	UFUNCTION(BlueprintPure, Category = "Settings Widget")
	bool GetToggleValue() const { return bCurrentValue; }

	UPROPERTY(BlueprintAssignable, Category = "SettingsWidget|Events")
	FOnToggleValueChanged OnToggleValueChanged;

protected:
	
	/** The toggle button widget */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Btn_ToggleButton;

	/** Text block inside the button showing current state */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))  // Optional for menu flexibility
	TObjectPtr<UCommonTextBlock> Text_ButtonLabel;
    
	/** Current toggle state */
	UPROPERTY(BlueprintReadOnly, Category = "Settings Widget")
	bool bCurrentValue;
    
	/** Default value for reset functionality */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings Widget")
	bool bDefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings Widget")
	TMap<EMCore_ToggleFunctionCallers,FString> SettingChangeFunctions;
	
	virtual void NativeConstruct() override;
	
	//~ Begin UMCore_SettingsWidget_Base Interface
	virtual void LoadValue() override;
	virtual void SaveValue() override;
	virtual void ResetToDefault() override;
	virtual bool IsModifiedFromDefault() override;
	virtual void UpdateVisualState_Implementation() override;
	//~ End UMCore_SettingsWidget_Base Interface

private:
	/** Handle toggle button clicked */
	UFUNCTION()
	void OnToggleButtonClicked();
};
