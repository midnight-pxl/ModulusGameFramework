// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "Input/UIActionBindingHandle.h"
#include "MCore_ActivatableBase.generated.h"

class UMCore_PDA_UITheme_Base;

/**
 * Blueprint-friendly wrapper for CommonUI's FUIActionBindingHandle.
 * Allows Blueprint code to receive and store binding handles for manual unregistration if needed.
 */
USTRUCT(BlueprintType)
struct MODULUSCORE_API FInputActionBindingHandle
{
	GENERATED_BODY()

	/** Internal handle to CommonUI's input binding system */
	FUIActionBindingHandle CommonHandle;

	/** Confirm handle is valid/registered */
	bool IsValid() const { return CommonHandle.IsValid(); }
};

/**
 * Dynamic delegate signature for input action callbacks.
 * Called when a registered input action is triggered.
 * 
 * @param ActionName - The row name from the input action data table
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FInputActionExecutedDelegate, FName, ActionName);

/**
 * 
 */
UCLASS(Abstract, Blueprintable, ClassGroup="ModulusUI", meta=(DisableNativeTick))
class MODULUSCORE_API UMCore_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UMCore_ActivatableBase(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI|Activation", meta=(Categories = "State"))
	FGameplayTagContainer BlockTags;
	
	UFUNCTION(BlueprintCallable, Category="UI|Input", meta=(AutoCreateRefTerm = "OverrideDisplayName"))
	void RegisterBinding(
		FDataTableRowHandle InputAction,
		const FInputActionExecutedDelegate& Callback,
		UPARAM(ref) FInputActionBindingHandle& IABindingHandle,
		FText OverrideDisplayName = FText::GetEmpty(),
		bool bShouldDisplayInActionBar = true);
	
	UFUNCTION(BlueprintCallable, Category="UI|Input")
	void UnregisterAllBindings();

#if WITH_EDITOR
	/**~ Begin UWidget Interface */
	/**
	 * Validates GetDesiredFocusTarget implementation at compile time.
	 * Prevents common mistake of missing focus target.
	 */
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree,
		class IWidgetCompilerLog& CompileLog) const override;
	/**~ End UWidget Interface */
#endif

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Theme")
	void ApplyTheme(UMCore_PDA_UITheme_Base* NewTheme);
	virtual void ApplyTheme_Implementation(UMCore_PDA_UITheme_Base* NewTheme);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Theme", meta = (DisplayName = "On Theme Applied"))
	void K2_OnThemeApplied(UMCore_PDA_UITheme_Base* Theme);
	
	bool bShouldBlockActivation() const;
	bool bShouldFocusOnActivation{false};

	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UMCore_PDA_UITheme_Base> CachedTheme;

private:
	/** Array of registered input bindings for auto cleanup */
	UPROPERTY(Transient)
	TArray<FUIActionBindingHandle> IABindingHandles;
	
	UFUNCTION()
	void HandleThemeChanged(UMCore_PDA_UITheme_Base* NewTheme);
	
	void BindThemeDelegate();
	void UnbindThemeDelegate();
	bool bThemeDelegateBound{false};
};
