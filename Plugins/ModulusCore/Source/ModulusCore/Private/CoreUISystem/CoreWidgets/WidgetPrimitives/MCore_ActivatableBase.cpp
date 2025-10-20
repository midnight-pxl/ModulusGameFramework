// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUISystem/CoreWidgets/WidgetPrimitives/MCore_ActivatableBase.h"

#if WITH_EDITOR
#include "Editor/WidgetCompilerLog.h"
#include "Blueprint/WidgetTree.h"
#endif

#define LOCTEXT_NAMESPACE "ModulusCoreUI"

UMCore_ActivatableBase::UMCore_ActivatableBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITOR
void UMCore_ActivatableBase::ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree,
                                                        class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);

	if (GetClass()->ClassGeneratedBy == nullptr) { return; }

	static const FName GetDesiredFocusTargetName =
		GET_FUNCTION_NAME_CHECKED(UMCore_ActivatableBase, GetDesiredFocusTarget);

	if (!GetClass()->IsFunctionImplementedInScript(GetDesiredFocusTargetName))
	{
		const UClass* ParentClass = GetClass()->GetSuperClass();
		if (ParentClass == UMCore_ActivatableBase::StaticClass())
		{
			CompileLog.Warning(LOCTEXT("MissingFocusTarget_Warning",
				"GetDesiredFocusTarget not implemented. Gamepad nav won't work correctly. "
				"Override this function and return a valid, focusable widget."));
		}
	}
}
#endif

#undef LOCTEXT_NAMESPACE
