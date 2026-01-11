// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUISystem/CoreWidgets/WidgetPrimitives/MCore_ActivatableBase.h"
#include "GameplayTagAssetInterface.h"
#include "CoreData/CoreLogging/LogModulusUI.h"
#include "Input/CommonUIInputTypes.h"


#if WITH_EDITOR
#include "Editor/WidgetCompilerLog.h"
#include "Blueprint/WidgetTree.h"
#endif

#define LOCTEXT_NAMESPACE "ModulusCoreUI"

UMCore_ActivatableBase::UMCore_ActivatableBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMCore_ActivatableBase::RegisterBinding(
	FDataTableRowHandle InputAction,
	const FInputActionExecutedDelegate& Callback,
	FInputActionBindingHandle& IABindingHandle,
	FText OverrideDisplayName,
	bool bShouldDisplayInActionBar)
{
	if (InputAction.IsNull() || InputAction.RowName.IsNone())
	{
		UE_LOG(LogModulusUI, Warning, TEXT("[%s] RegisterBinding: InputAction row handle is invalid"),
			*GetName());
		return;
	}
	
	if (!Callback.IsBound())
	{
		UE_LOG(LogModulusUI, Warning, TEXT("[%s] RegisterBinding: Callback delegate not bound"),
			*GetName());
		return;
	}
	
	FBindUIActionArgs BindArgs(InputAction, FSimpleDelegate::CreateLambda([InputAction, Callback]()
	{
		Callback.ExecuteIfBound(InputAction.RowName);
	}));
	
	BindArgs.bDisplayInActionBar = bShouldDisplayInActionBar;
	
	if (!OverrideDisplayName.IsEmpty())
	{
		BindArgs.OverrideDisplayName = OverrideDisplayName;
	}
	
	FUIActionBindingHandle NewHandle = RegisterUIActionBinding(BindArgs);
	
	IABindingHandle.CommonHandle = NewHandle;
	
	IABindingHandles.Add(NewHandle);
	
	UE_LOG(LogModulusUI, Log, 
	TEXT("[%s] Registered input binding for action: %s (Display in ActionBar set to %s)"),
		*GetName(), 
		*InputAction.RowName.ToString(),
		bShouldDisplayInActionBar ? TEXT("Yes") : TEXT("No"));
}

void UMCore_ActivatableBase::UnregisterAllBindings()
{
	if (IABindingHandles.Num() == 0) { return; }
	
	UE_LOG(LogModulusUI, Log, TEXT("UnregisterAllBindings: Unregistering input binding(s) for %s"),
		*GetName());
	
	/** Unregister all tracked input bindings */
	for (FUIActionBindingHandle& Handle : IABindingHandles)
	{
		if (Handle.IsValid()){ Handle.Unregister(); }
	}

	/** Clear IABindingHandles array */
	IABindingHandles.Empty();
}

void UMCore_ActivatableBase::NativeOnActivated()
{
	/** Check OwningPlayer for BlockTags prior to allowing activation */
	if (bShouldBlockActivation())
	{
		UE_LOG(LogModulusUI, Log, TEXT("NativeOnActivated: Activation blocked by BlockTags, deactivating..."));

		DeactivateWidget();
		return;
	}

	/** No BlockTags, activate normally */
	Super::NativeOnActivated();

	/** Autofocus target widget */
	if (bShouldFocusOnActivation)
	{
		if (UWidget* WidgetToFocus = NativeGetDesiredFocusTarget())
		{
			WidgetToFocus->SetFocus();
			
			UE_LOG(LogModulusUI, Log, TEXT("NativeOnActivated: Auto focusing on widget %s"),
				*GetName());
			return;
		}
	}
	
	UE_LOG(LogModulusUI, Log, TEXT("NativeOnActivated: Widget %s not set to autofocus"),
	*GetName());
}

void UMCore_ActivatableBase::NativeOnDeactivated()
{
	/** Cleanup own input bindings BEFORE calling Super (memory leaks) */
	UnregisterAllBindings();
	
	Super::NativeOnDeactivated();
	
	UE_LOG(LogModulusUI, Log, TEXT("Widget %s deactivated, input bindings cleared"),
		*GetName());
}

bool UMCore_ActivatableBase::bShouldBlockActivation() const
{
	if (BlockTags.IsEmpty()) { return false; }

	/** Get OwningPlayerController */
	const APlayerController* OwningPlayer = GetOwningPlayer();
	if (!OwningPlayer)
	{
		/** No PlayerController found (Shouldn't Occur) - allow activation */
		UE_LOG(LogModulusUI, Warning, TEXT("bShouldBlockActivation: No PlayerController detected...?"));
		return false;
	}

	const IGameplayTagAssetInterface* BlockTagInterface = Cast<IGameplayTagAssetInterface>(OwningPlayer);
	if (!BlockTagInterface)
	{
		/** IMCore_BlockingTagInterface not implemented - allow activation */
		UE_LOG(LogModulusUI, Verbose, TEXT("bShouldBlockActivation: OwningPlayer doesn't implement the Modulus BlockTag Interface, allow"));
		return false;
	}

	/** Get OwningPlayer's BlockTags */
	FGameplayTagContainer OwnedTags;
	BlockTagInterface->GetOwnedGameplayTags(OwnedTags);

	/** Block activation if OwningPlayer has any matching BlockTags */
	if (OwnedTags.HasAny(BlockTags))
	{
		UE_LOG(LogModulusUI, Log, TEXT("bShouldBlockActivation: Activation blocked by BlockTag: %s, PlayerTags: %s"),
			*BlockTags.ToStringSimple(),
			*OwnedTags.ToStringSimple());
		return true;
	}

	/** No blocking tags found - allow activation */
	return false;
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
