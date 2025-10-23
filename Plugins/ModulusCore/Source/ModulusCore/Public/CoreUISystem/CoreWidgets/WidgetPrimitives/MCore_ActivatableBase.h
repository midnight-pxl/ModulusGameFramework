// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MCore_ActivatableBase.generated.h"

class UMCore_DA_UITheme_Base;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, ClassGroup="ModulusUI")
class MODULUSCORE_API UMCore_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UMCore_ActivatableBase(const FObjectInitializer& ObjectInitializer);

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
	friend class IMCore_ThemeableInterface;

	UPROPERTY(Transient)
	mutable TWeakObjectPtr<UMCore_DA_UITheme_Base> CachedTheme;
};
