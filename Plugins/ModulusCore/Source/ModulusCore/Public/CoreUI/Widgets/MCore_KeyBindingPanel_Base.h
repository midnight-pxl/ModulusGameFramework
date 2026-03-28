// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MCore_KeyBindingPanel_Base.generated.h"

/**
 * Base class for key binding panel content displayed inline in the Settings Panel.
 * Subclass in Blueprint to build out the rebinding UI.
 */
UCLASS(Abstract, Blueprintable)
class MODULUSCORE_API UMCore_KeyBindingPanel_Base : public UCommonUserWidget
{
	GENERATED_BODY()
};
