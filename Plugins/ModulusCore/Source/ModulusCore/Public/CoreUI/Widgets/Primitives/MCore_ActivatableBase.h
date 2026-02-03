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
 * Base activatable widget with theme integration, input binding management, and BlockTag support.
 * Automatically cleans up input bindings on deactivation to prevent memory leaks.
 */
UCLASS(Abstract, Blueprintable, ClassGroup="ModulusUI", meta=(DisableNativeTick))
class MODULUSCORE_API UMCore_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UMCore_ActivatableBase(const FObjectInitializer& ObjectInitializer);

	/** GameplayTags that block this widget from activating when present on OwningPlayer */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI|Activation", meta=(Categories = "State"))
	FGameplayTagContainer BlockTags;

	/**
	 * Register an input action binding with automatic cleanup on deactivation.
	 * @param InputAction - DataTable row reference to the input action
	 * @param Callback - Delegate called when action triggers
	 * @param IABindingHandle - Output handle for manual unregistration if needed
	 * @param OverrideDisplayName - Optional display name override for action bar
	 * @param bShouldDisplayInActionBar - Whether to show in CommonUI action bar
	 */
	UFUNCTION(BlueprintCallable, Category="UI|Input", meta=(AutoCreateRefTerm = "OverrideDisplayName"))
	void RegisterBinding(
		FDataTableRowHandle InputAction,
		const FInputActionExecutedDelegate& Callback,
		UPARAM(ref) FInputActionBindingHandle& IABindingHandle,
		FText OverrideDisplayName = FText::GetEmpty(),
		bool bShouldDisplayInActionBar = true);

	/** Unregister all input bindings tracked by this widget */
	UFUNCTION(BlueprintCallable, Category="UI|Input")
	void UnregisterAllBindings();

#if WITH_EDITOR
	//~ Begin UWidget Interface
	/**
	 * Validates GetDesiredFocusTarget implementation at compile time.
	 * Prevents common mistake of missing focus target.
	 */
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree,
		class IWidgetCompilerLog& CompileLog) const override;
	//~ End UWidget Interface
#endif

protected:
	//~ Begin UUserWidget Interface
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual void NativeDestruct() override;
	//~ End UUserWidget Interface

	/**
	 * Apply theme styling to this widget. Override for custom theme handling.
	 * @param NewTheme - Theme to apply, may be nullptr
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Theme")
	void ApplyTheme(UMCore_PDA_UITheme_Base* NewTheme);
	virtual void ApplyTheme_Implementation(UMCore_PDA_UITheme_Base* NewTheme);

	/** Blueprint event fired after theme is applied */
	UFUNCTION(BlueprintImplementableEvent, Category = "Theme", meta = (DisplayName = "On Theme Applied"))
	void K2_OnThemeApplied(UMCore_PDA_UITheme_Base* Theme);

	/** Check if activation should be blocked based on OwningPlayer's tags */
	bool bShouldBlockActivation() const;

	/** When true, automatically focuses GetDesiredFocusTarget on activation */
	bool bShouldFocusOnActivation{false};

	/** Cached theme reference for quick access */
	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UMCore_PDA_UITheme_Base> CachedTheme;

private:
	/** Tracked input bindings for automatic cleanup on deactivation */
	UPROPERTY(Transient)
	TArray<FUIActionBindingHandle> IABindingHandles;

	/** Callback for UISubsystem theme change delegate */
	UFUNCTION()
	void HandleThemeChanged(UMCore_PDA_UITheme_Base* NewTheme);

	/** Subscribe to theme change notifications */
	void BindThemeDelegate();

	/** Unsubscribe from theme change notifications */
	void UnbindThemeDelegate();

	/** Tracks whether theme delegate is currently bound */
	bool bThemeDelegateBound{false};
};
