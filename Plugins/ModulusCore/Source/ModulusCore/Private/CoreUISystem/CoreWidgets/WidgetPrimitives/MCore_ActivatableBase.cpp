// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUISystem/CoreWidgets/WidgetPrimitives/MCore_ActivatableBase.h"
#include "GameplayTagAssetInterface.h"
#include "CoreData/CoreLogging/LogModulusUI.h"

#if WITH_EDITOR
#include "Editor/WidgetCompilerLog.h"
#include "Blueprint/WidgetTree.h"
#endif

#define LOCTEXT_NAMESPACE "ModulusCoreUI"

UMCore_ActivatableBase::UMCore_ActivatableBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMCore_ActivatableBase::NativeOnActivated()
{
	// Check OwningPlayer for BlockTags prior to allowing activation
	if (bShouldBlockActivation())
	{
		UE_LOG(LogModulusUI, Log, TEXT("NativeOnActivated: Activation blocked by BlockTags, deactivating..."));
		
		DeactivateWidget();
		return;
	}
	
	// No BlockTags, activate normally
	Super::NativeOnActivated();
}

bool UMCore_ActivatableBase::bShouldBlockActivation() const
{
	if (BlockTags.IsEmpty()) { return false; }
	
	// Get OwningPlayerController
	const APlayerController* OwningPlayer = GetOwningPlayer();
	if (!OwningPlayer)
	{
		// No PlayerController found (Shouldn't Occur) - allow activation
		UE_LOG(LogModulusUI, Warning, TEXT("bShouldBlockActivation: No PlayerController detected...?"));
		return false;
	}
	
	const IGameplayTagAssetInterface* BlockTagInterface = Cast<IGameplayTagAssetInterface>(OwningPlayer);
	if (!BlockTagInterface)
	{
		// IMCore_BlockingTagInterface not implemented - allow activation
		UE_LOG(LogModulusUI, Verbose, TEXT("bShouldBlockActivation: OwningPlayer doesn't implement the Modulus BlockTag Interface, allow"));
		return false;
	}
	
	// Get OwningPlayer's BlockTags
	FGameplayTagContainer OwnedTags;
	BlockTagInterface->GetOwnedGameplayTags(OwnedTags);
	
	// Block activation if OwningPlayer has any matching BlockTags
	if (OwnedTags.HasAny(BlockTags))
	{
		UE_LOG(LogModulusUI, Log, TEXT("bShouldBlockActivation: Activation blocked by BlockTag: %s, PlayerTags: %s"),
			*BlockTags.ToStringSimple(),
			*OwnedTags.ToStringSimple());
		return true;
	}
	
	// No blocking tags found - allow activation
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
