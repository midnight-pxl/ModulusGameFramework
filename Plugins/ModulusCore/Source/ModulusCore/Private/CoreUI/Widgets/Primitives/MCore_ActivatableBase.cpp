// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUI/Widgets/Primitives/MCore_ActivatableBase.h"
#include "GameplayTagAssetInterface.h"
#include "CoreData/Assets/UI/Themes/MCore_PDA_UITheme_Base.h"
#include "CoreData/Logging/LogModulusUI.h"
#include "CoreData/DevSettings/MCore_CoreSettings.h"
#include "CoreUI/MCore_UISubsystem.h"
#include "Input/CommonUIInputTypes.h"

#if WITH_EDITOR
#include "Editor/WidgetCompilerLog.h"
#include "Blueprint/WidgetTree.h"
#endif

#define LOCTEXT_NAMESPACE "ModulusCoreUI"

UMCore_ActivatableBase::UMCore_ActivatableBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoActivate = false;
}

void UMCore_ActivatableBase::NativeConstruct()
{
	// Widget stacks manage activation via HandleActiveIndexChanged -> ActivateWidget().
	// If bAutoActivate is true, NativeConstruct auto-activates BEFORE the stack calls
	// ActivateWidget, making the stack's call a no-op (bIsActive already true).
	// This silently prevents NativeOnActivated and BP_OnActivated from ever firing.
	if (bAutoActivate)
	{
		UE_LOG(LogModulusUI, Error,
			TEXT("[%s] bAutoActivate is TRUE - stack-managed widgets must not auto-activate. Forcing to false."),
			*GetName());
		bAutoActivate = false;
	}

	Super::NativeConstruct();
}

/**
 * @param InputAction          DataTable row reference to the input action to bind.
 * @param Callback             Delegate fired when the action triggers.
 * @param IABindingHandle      Output handle for optional manual unregistration.
 * @param OverrideDisplayName  Display name override for the CommonUI action bar. Empty uses default.
 * @param bShouldDisplayInActionBar  Whether this binding appears in the CommonUI action bar.
 */
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

	for (FUIActionBindingHandle& Handle : IABindingHandles)
	{
		if (Handle.IsValid()){ Handle.Unregister(); }
	}

	IABindingHandles.Empty();
}

void UMCore_ActivatableBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Apply design-time theme for UMG editor preview. Runtime re-applies from UISubsystem.
	ApplyTheme(UMCore_CoreSettings::GetDesignTimeTheme());
}

void UMCore_ActivatableBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Guard: Reset stale bIsActive from Blueprint CDO serialization.
	// bIsActive is a non-Transient UPROPERTY in CommonActivatableWidget.
	// If a Blueprint was saved while its widget preview was in an active state,
	// bIsActive=true gets baked into the CDO. Every new instance then starts
	// "already activated", causing the stack's ActivateWidget() to silently
	// skip InternalProcessActivation -> NativeOnActivated -> BP_OnActivated.
	if (IsActivated())
	{
		UE_LOG(LogModulusUI, Log,
			TEXT("[%s] Stale bIsActive detected in CDO - resetting for stack-managed activation"),
			*GetName());
		Reset();
	}

	BindThemeDelegate();

	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (UMCore_UISubsystem* UI = LocalPlayer->GetSubsystem<UMCore_UISubsystem>())
		{
			ApplyTheme(UI->GetActiveTheme());
		}
	}
}

void UMCore_ActivatableBase::NativeOnActivated()
{
	if (bShouldBlockActivation())
	{
		UE_LOG(LogModulusUI, Log, TEXT("NativeOnActivated: Activation blocked by BlockTags, deactivating..."));

		DeactivateWidget();
		return;
	}

	Super::NativeOnActivated();

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
	// Cleanup input bindings BEFORE calling Super to prevent memory leaks
	UnregisterAllBindings();

	Super::NativeOnDeactivated();

	UE_LOG(LogModulusUI, Log, TEXT("[%s] Deactivated, input bindings cleared"),
		*GetName());
}

bool UMCore_ActivatableBase::bShouldBlockActivation() const
{
	if (BlockTags.IsEmpty()) { return false; }

	const APlayerController* OwningPlayer = GetOwningPlayer();
	if (!OwningPlayer)
	{
		UE_LOG(LogModulusUI, Warning, TEXT("bShouldBlockActivation: No PlayerController detected...?"));
		return false;
	}

	const IGameplayTagAssetInterface* BlockTagInterface = Cast<IGameplayTagAssetInterface>(OwningPlayer);
	if (!BlockTagInterface)
	{
		UE_LOG(LogModulusUI, Verbose, TEXT("bShouldBlockActivation: OwningPlayer doesn't implement the Modulus BlockTag Interface, allow"));
		return false;
	}

	FGameplayTagContainer OwnedTags;
	BlockTagInterface->GetOwnedGameplayTags(OwnedTags);

	if (OwnedTags.HasAny(BlockTags))
	{
		UE_LOG(LogModulusUI, Log, TEXT("bShouldBlockActivation: Activation blocked by BlockTag: %s, PlayerTags: %s"),
			*BlockTags.ToStringSimple(),
			*OwnedTags.ToStringSimple());
		return true;
	}

	return false;
}

void UMCore_ActivatableBase::ApplyTheme_Implementation(UMCore_PDA_UITheme_Base* Theme)
{
	K2_OnThemeApplied(Theme);
}

void UMCore_ActivatableBase::HandleThemeChanged(UMCore_PDA_UITheme_Base* NewTheme)
{
	CachedTheme = NewTheme;
	ApplyTheme(NewTheme);
}

void UMCore_ActivatableBase::BindThemeDelegate()
{
	if (bThemeDelegateBound) { return; }

	ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
	if (!LocalPlayer) { return; }

	UMCore_UISubsystem* UI = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
	if (!UI) { return; }

	UI->OnThemeChanged.AddDynamic(this, &UMCore_ActivatableBase::HandleThemeChanged);
	bThemeDelegateBound = true;
}

void UMCore_ActivatableBase::UnbindThemeDelegate()
{
	if (!bThemeDelegateBound) { return; }

	ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
	if (!LocalPlayer)
	{
		bThemeDelegateBound = false;
		return;
	}

	UMCore_UISubsystem* UI = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
	if (UI)
	{
		UI->OnThemeChanged.RemoveDynamic(this, &UMCore_ActivatableBase::HandleThemeChanged);
	}

	bThemeDelegateBound = false;
}

void UMCore_ActivatableBase::NativeDestruct()
{
	UnbindThemeDelegate();
	Super::NativeDestruct();
}

#if WITH_EDITOR
void UMCore_ActivatableBase::ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree,
                                                        class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);

	if (GetClass()->ClassGeneratedBy == nullptr) { return; }

	// CommonUI exposes BP_GetDesiredFocusTarget as the Blueprint-overridable function
	static const FName BPGetDesiredFocusTargetName = TEXT("BP_GetDesiredFocusTarget");

	if (!GetClass()->IsFunctionImplementedInScript(BPGetDesiredFocusTargetName))
	{
		// Only warn for direct children - intermediate C++ classes may implement it natively
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
