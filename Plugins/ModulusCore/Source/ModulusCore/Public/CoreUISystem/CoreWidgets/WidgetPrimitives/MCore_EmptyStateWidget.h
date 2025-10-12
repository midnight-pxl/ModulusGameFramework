// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MCore_EmptyStateWidget.generated.h"

/**
 * 
 */
UCLASS()
class MODULUSCORE_API UMCore_EmptyStateWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:

	virtual UWidget* NativeGetDesiredFocusTarget() const override
	{
		// Empty state widget has no interactive elements
		return nullptr;
	}
};
