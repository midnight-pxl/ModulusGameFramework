// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_ActivatableBase.h
 *
 * Base activatable widget with theme integration, input binding management,
 * and BlockTag support. Automatically cleans up input bindings on deactivation.
 */

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

	FUIActionBindingHandle CommonHandle;

	bool IsValid() const { return CommonHandle.IsValid(); }
};

/**
 * Dynamic delegate signature for input action callbacks.
 *
 * @param ActionName - The row name from the input action data table
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FInputActionExecutedDelegate, FName, ActionName);

/**
 * Base activatable widget with theme integration, input binding management, and BlockTag support.
 *
 * Key Features:
 * - Automatic input binding cleanup on deactivation (prevents memory leaks)
 * - Theme system integration via UISubsystem::OnThemeChanged
 * - BlockTag support to conditionally prevent activation
 * - Compile-time validation for GetDesiredFocusTarget implementation
 *
 * Blueprint Usage:
 * 1. Create Blueprint extending this class
 * 2. Use RegisterBinding() to bind input actions with automatic cleanup
 * 3. Override ApplyTheme or bind OnThemeApplied for theme changes
 * 4. Set BlockTags to prevent activation under specific game states
 */
UCLASS(Abstract, Blueprintable, ClassGroup="ModulusUI", meta=(DisableNativeTick))
class MODULUSCORE_API UMCore_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UMCore_ActivatableBase(const FObjectInitializer& ObjectInitializer);

	/* GameplayTags that block this widget from activating when present on OwningPlayer */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI|Activation", meta=(Categories = "State"))
	FGameplayTagContainer BlockTags;

	/**
	 * Register an input action binding with automatic cleanup on deactivation.
	 *
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

	/**
	 * Unregister all input bindings tracked by this widget.
	 * Called automatically on deactivation. Safe to call manually.
	 */
	UFUNCTION(BlueprintCallable, Category="UI|Input")
	void UnregisterAllBindings();

#if WITH_EDITOR
	// ============================================================================
	// EDITOR VALIDATION
	// ============================================================================

	/**
	 * Validates GetDesiredFocusTarget implementation at compile time.
	 * Prevents common mistake of missing focus target for gamepad navigation.
	 */
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree,
		class IWidgetCompilerLog& CompileLog) const override;
#endif

protected:
	// ============================================================================
	// LIFECYCLE
	// ============================================================================

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual void NativeDestruct() override;

	// ============================================================================
	// THEME
	// ============================================================================

	/**
	 * Apply theme styling to this widget. Override for custom theme handling.
	 *
	 * @param NewTheme - Theme to apply, may be nullptr
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Theme")
	void ApplyTheme(UMCore_PDA_UITheme_Base* NewTheme);
	virtual void ApplyTheme_Implementation(UMCore_PDA_UITheme_Base* NewTheme);

	UFUNCTION(BlueprintImplementableEvent, Category = "Theme", meta = (DisplayName = "On Theme Applied"))
	void K2_OnThemeApplied(UMCore_PDA_UITheme_Base* Theme);

	/* Check if activation should be blocked based on OwningPlayer's tags */
	bool bShouldBlockActivation() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI|Activation")
	bool bShouldFocusOnActivation{false};

	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UMCore_PDA_UITheme_Base> CachedTheme;

private:
	UPROPERTY(Transient)
	TArray<FUIActionBindingHandle> IABindingHandles;

	UFUNCTION()
	void HandleThemeChanged(UMCore_PDA_UITheme_Base* NewTheme);

	void BindThemeDelegate();
	void UnbindThemeDelegate();

	bool bThemeDelegateBound{false};
};
