// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreUISystem/MCore_UIExtensionSubsystem.h"
#include "CoreData/CoreStructEnums/UIStructsEnums/UIExtensionStructEnums/MCore_UIExtensionTypes.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogMCoreUIExtension, Log, All);

//=============================================================================
// USubsystem Interface
//=============================================================================

void UMCore_UIExtensionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	NextHandleID = 1;
	
	UE_LOG(LogMCoreUIExtension, Verbose, TEXT("UIExtension subsystem initialized"));
}

void UMCore_UIExtensionSubsystem::Deinitialize()
{
	ExtensionPointMap.Empty();
	ExtensionMap.Empty();
	ExtensionPointHandles.Empty();
	ExtensionHandles.Empty();
	NextHandleID = 1;
	
	UE_LOG(LogMCoreUIExtension, Verbose, TEXT("UIExtension subsystem deinitialized"));
	
	Super::Deinitialize();
}

//=============================================================================
// Extension Point Registration
//=============================================================================

FMCore_UIExtensionPointHandle UMCore_UIExtensionSubsystem::RegisterExtensionPoint(
	const FGameplayTag& ExtensionPointTag,
	UObject* ContextObject,
	const FOnUIExtensionChanged& ExtensionCallback)
{
	if (!ExtensionPointTag.IsValid())
	{
		UE_LOG(LogMCoreUIExtension, Error, 
			TEXT("RegisterExtensionPoint: Invalid ExtensionPointTag"));
		return FMCore_UIExtensionPointHandle();
	}

	if (!ExtensionCallback.IsBound())
	{
		UE_LOG(LogMCoreUIExtension, Warning,
			TEXT("RegisterExtensionPoint: Callback not bound for tag '%s'"), 
			*ExtensionPointTag.ToString());
	}

	TSharedPtr<FMCore_UIExtensionPoint> ExtensionPoint = MakeShared<FMCore_UIExtensionPoint>();
	ExtensionPoint->ExtensionPointTag = ExtensionPointTag;
	ExtensionPoint->ContextObject = ContextObject;
	ExtensionPoint->OnChangeCallback = ExtensionCallback;

	const uint32 HandleID = NextHandleID++;
	
	ExtensionPointMap.FindOrAdd(ExtensionPointTag).Add(ExtensionPoint);
	ExtensionPointHandles.Add(HandleID, ExtensionPoint);

	NotifyExtensionPointOfExtensions(ExtensionPoint.Get());

	UE_LOG(LogMCoreUIExtension, Verbose,
		TEXT("Registered extension point: Tag='%s', Context=%s, HandleID=%u"),
		*ExtensionPointTag.ToString(),
		ContextObject ? *ContextObject->GetName() : TEXT("Global"),
		HandleID);

	return FMCore_UIExtensionPointHandle(HandleID);
}

void UMCore_UIExtensionSubsystem::UnregisterExtensionPoint(
	FMCore_UIExtensionPointHandle& ExtensionPointHandle)
{
	if (!ExtensionPointHandle.IsValid())
	{
		return;
	}

	const uint32 HandleID = ExtensionPointHandle.GetID();

	TSharedPtr<FMCore_UIExtensionPoint>* FoundPoint = ExtensionPointHandles.Find(HandleID);
	if (!FoundPoint)
	{
		UE_LOG(LogMCoreUIExtension, Warning,
			TEXT("UnregisterExtensionPoint: Handle %u not found"), HandleID);
		ExtensionPointHandle.Invalidate();
		return;
	}

	const FGameplayTag Tag = (*FoundPoint)->ExtensionPointTag;

	if (TArray<TSharedPtr<FMCore_UIExtensionPoint>>* PointArray = ExtensionPointMap.Find(Tag))
	{
		PointArray->Remove(*FoundPoint);
		if (PointArray->Num() == 0)
		{
			ExtensionPointMap.Remove(Tag);
		}
	}

	ExtensionPointHandles.Remove(HandleID);
	ExtensionPointHandle.Invalidate();

	UE_LOG(LogMCoreUIExtension, Verbose,
		TEXT("Unregistered extension point: HandleID=%u"), HandleID);
}

//=============================================================================
// Extension Registration
//=============================================================================

FMCore_UIExtensionHandle UMCore_UIExtensionSubsystem::RegisterExtension(
	const FGameplayTag& ExtensionPointTag,
	UObject* ContextObject,
	TSubclassOf<UUserWidget> WidgetClass,
	int32 Priority)
{
	if (!ExtensionPointTag.IsValid())
	{
		UE_LOG(LogMCoreUIExtension, Error,
			TEXT("RegisterExtension: Invalid ExtensionPointTag"));
		return FMCore_UIExtensionHandle();
	}

	if (!WidgetClass)
	{
		UE_LOG(LogMCoreUIExtension, Error,
			TEXT("RegisterExtension: Invalid WidgetClass for tag '%s'"),
			*ExtensionPointTag.ToString());
		return FMCore_UIExtensionHandle();
	}

	TSharedPtr<FMCore_UIExtension> Extension = MakeShared<FMCore_UIExtension>();
	Extension->ExtensionPointTag = ExtensionPointTag;
	Extension->ContextObject = ContextObject;
	Extension->WidgetClass = WidgetClass;
	Extension->Priority = Priority;

	const uint32 HandleID = NextHandleID++;
	Extension->HandleID = HandleID;  // Store handle ID in extension

	ExtensionMap.FindOrAdd(ExtensionPointTag).Add(Extension);
	ExtensionHandles.Add(HandleID, Extension);

	NotifyExtensionPointsOfExtension(EMCore_UIExtensionAction::Added, Extension.Get());

	UE_LOG(LogMCoreUIExtension, Verbose,
		TEXT("Registered extension: Tag='%s', Widget=%s, Priority=%d, Context=%s, HandleID=%u"),
		*ExtensionPointTag.ToString(),
		*WidgetClass->GetName(),
		Priority,
		ContextObject ? *ContextObject->GetName() : TEXT("Global"),
		HandleID);

	return FMCore_UIExtensionHandle(HandleID);
}

void UMCore_UIExtensionSubsystem::UnregisterExtension(FMCore_UIExtensionHandle& ExtensionHandle)
{
	if (!ExtensionHandle.IsValid())
	{
		return;
	}

	const uint32 HandleID = ExtensionHandle.GetID();

	TSharedPtr<FMCore_UIExtension>* FoundExtension = ExtensionHandles.Find(HandleID);
	if (!FoundExtension)
	{
		UE_LOG(LogMCoreUIExtension, Warning,
			TEXT("UnregisterExtension: Handle %u not found"), HandleID);
		ExtensionHandle.Invalidate();
		return;
	}

	const FGameplayTag Tag = (*FoundExtension)->ExtensionPointTag;

	NotifyExtensionPointsOfExtension(EMCore_UIExtensionAction::Removed, FoundExtension->Get());

	if (TArray<TSharedPtr<FMCore_UIExtension>>* ExtArray = ExtensionMap.Find(Tag))
	{
		ExtArray->Remove(*FoundExtension);
		if (ExtArray->Num() == 0)
		{
			ExtensionMap.Remove(Tag);
		}
	}

	ExtensionHandles.Remove(HandleID);
	ExtensionHandle.Invalidate();

	UE_LOG(LogMCoreUIExtension, Verbose,
		TEXT("Unregistered extension: HandleID=%u"), HandleID);
}

//=============================================================================
// Query Functions
//=============================================================================

TSubclassOf<UUserWidget> UMCore_UIExtensionSubsystem::GetWidgetClassForExtension(
	const FMCore_UIExtensionHandle& ExtensionHandle) const
{
	if (!ExtensionHandle.IsValid())
	{
		return nullptr;
	}

	const TSharedPtr<FMCore_UIExtension>* FoundExtension = ExtensionHandles.Find(ExtensionHandle.GetID());
	if (!FoundExtension)
	{
		return nullptr;
	}

	return (*FoundExtension)->WidgetClass;
}

bool UMCore_UIExtensionSubsystem::IsExtensionRegistered(
	const FMCore_UIExtensionHandle& ExtensionHandle) const
{
	if (!ExtensionHandle.IsValid())
	{
		return false;
	}

	return ExtensionHandles.Contains(ExtensionHandle.GetID());
}

bool UMCore_UIExtensionSubsystem::IsExtensionPointRegistered(
	const FMCore_UIExtensionPointHandle& ExtensionPointHandle) const
{
	if (!ExtensionPointHandle.IsValid())
	{
		return false;
	}

	return ExtensionPointHandles.Contains(ExtensionPointHandle.GetID());
}

//=============================================================================
// Internal Notification System
//=============================================================================

void UMCore_UIExtensionSubsystem::NotifyExtensionPointsOfExtension(
	EMCore_UIExtensionAction Action,
	const FMCore_UIExtension* Extension)
{
	if (!Extension)
	{
		return;
	}

	const FGameplayTag Tag = Extension->ExtensionPointTag;

	const TArray<TSharedPtr<FMCore_UIExtensionPoint>>* Points = ExtensionPointMap.Find(Tag);
	if (!Points)
	{
		return;
	}

	// Use stored handle ID (no reverse lookup needed)
	FMCore_UIExtensionHandle Handle(Extension->HandleID);

	for (const TSharedPtr<FMCore_UIExtensionPoint>& Point : *Points)
	{
		if (Point->DoesExtensionPassContract(Extension))
		{
			Point->OnChangeCallback.ExecuteIfBound(Action, Handle);
		}
	}
}

void UMCore_UIExtensionSubsystem::NotifyExtensionPointOfExtensions(
	const FMCore_UIExtensionPoint* ExtensionPoint)
{
	if (!ExtensionPoint)
	{
		return;
	}

	const FGameplayTag Tag = ExtensionPoint->ExtensionPointTag;

	const TArray<TSharedPtr<FMCore_UIExtension>>* Extensions = ExtensionMap.Find(Tag);
	if (!Extensions || Extensions->Num() == 0)
	{
		return;
	}

	// Sort by priority (higher first) - fixes Lyra bug where they store but never sort
	TArray<TSharedPtr<FMCore_UIExtension>> SortedExtensions = *Extensions;
	SortedExtensions.Sort([](const TSharedPtr<FMCore_UIExtension>& A,
	                         const TSharedPtr<FMCore_UIExtension>& B)
	{
		return A->Priority > B->Priority;
	});

	for (const TSharedPtr<FMCore_UIExtension>& Extension : SortedExtensions)
	{
		if (ExtensionPoint->DoesExtensionPassContract(Extension.Get()))
		{
			// Use stored handle ID (no reverse lookup needed)
			FMCore_UIExtensionHandle Handle(Extension->HandleID);
			ExtensionPoint->OnChangeCallback.ExecuteIfBound(EMCore_UIExtensionAction::Added, Handle);
		}
	}
}