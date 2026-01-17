// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MCore_UIExtensionTypes.generated.h"

class UUserWidget;
class UMCore_UIExtensionSubsystem;

/**
 * Action taken on a UI extension (added or removed from extension point)
 */
UENUM(BlueprintType)
enum class EMCore_UIExtensionAction : uint8
{
	/** Extension was added to extension point */
	Added,
	/** Extension was removed from extension point */
	Removed	
};

/**
 * Opaque handle referencing a UI extension.
 * Similar to FTimerHandle - contains only an ID for subsystem lookup.
 * 
 * Unregistration: Call Subsystem->UnregisterExtension(Handle) explicitly.
 * 
 * Usage:
 *   FMCore_UIExtensionHandle Handle = Subsystem->RegisterExtension(...);
 *   // ... later ...
 *   Subsystem->UnregisterExtension(Handle);
 */
USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_UIExtensionHandle
{
	GENERATED_BODY()

public:
	/** Default constructor - creates invalid handle */
	FMCore_UIExtensionHandle()
		: HandleID(0)
	{}
	
	/** Constructor with ID - used internally by subsystem */
	explicit FMCore_UIExtensionHandle(uint32 InID)
		: HandleID(InID)
	{}

	/** Check if handle is valid (non-zero ID) */
	bool IsValid() const { return HandleID != 0; }

	/** Get unique handle ID (used by subsystem for lookup) */
	uint32 GetID() const { return HandleID; }

	/** Reset handle to invalid state (ID = 0) */
	void Invalidate() { HandleID = 0; }

	/** Equality operator for handle comparison */
	bool operator==(const FMCore_UIExtensionHandle& Other) const { return HandleID == Other.HandleID; }

	/** Inequality operator */
	bool operator!=(const FMCore_UIExtensionHandle& Other) const { return HandleID != Other.HandleID; }

private:
	/** Unique identifier for this extension (0 = invalid) */
	UPROPERTY()
	uint32 HandleID;
};

/**
 * Opaque handle referencing a UI extension point.
 * Similar to FTimerHandle - contains only an ID for subsystem lookup.
 * 
 * Unregistration: Call Subsystem->UnregisterExtensionPoint(Handle) explicitly.
 * 
 * Usage:
 *   FMCore_UIExtensionPointHandle Handle = Subsystem->RegisterExtensionPoint(...);
 *   // ... later ...
 *   Subsystem->UnregisterExtensionPoint(Handle);
 */
USTRUCT(BlueprintType)
struct MODULUSCORE_API FMCore_UIExtensionPointHandle
{
	GENERATED_BODY()

public:
	/** Default constructor - creates invalid handle */
	FMCore_UIExtensionPointHandle()
		: HandleID(0)
	{}
	
	/** Constructor with ID - used internally by subsystem */
	explicit FMCore_UIExtensionPointHandle(uint32 InID)
		: HandleID(InID)
	{}

	/** Check if handle is valid (non-zero ID) */
	bool IsValid() const { return HandleID != 0; }

	/** Get unique handle ID (used by subsystem for lookup) */
	uint32 GetID() const { return HandleID; }

	/** Reset handle to invalid state (ID = 0) */
	void Invalidate() { HandleID = 0; }

	/** Equality operator for handle comparison */
	bool operator==(const FMCore_UIExtensionPointHandle& Other) const { return HandleID == Other.HandleID; }

	/** Inequality operator */
	bool operator!=(const FMCore_UIExtensionPointHandle& Other) const { return HandleID != Other.HandleID; }

private:
	/** Unique identifier for this extension point (0 = invalid) */
	UPROPERTY()
	uint32 HandleID;
};

/**
 * Dynamic delegate for Blueprint support.
 * Invoked when extensions are added/removed from an extension point.
 * 
 * @param Action - Whether extension was added or removed
 * @param ExtensionHandle - Handle to the extension (query widget class from subsystem)
 */
DECLARE_DYNAMIC_DELEGATE_TwoParams(
	FOnUIExtensionChanged,
	EMCore_UIExtensionAction, Action,
	FMCore_UIExtensionHandle, ExtensionHandle);

/**
 * Internal data structure representing a UI extension.
 * Extensions are widgets that want to be injected into extension points.
 * 
 * Not exposed to Blueprint - managed by subsystem.
 */
struct FMCore_UIExtension
{
	/** GameplayTag identifying which extension point(s) this targets */
	FGameplayTag ExtensionPointTag;
	
	/** 
	 * Sort priority - higher values appear first.
	 * Default: 0
	 * Example: Priority 100 appears before Priority 50
	 */
	int32 Priority{0};
	
	/**
	 * Optional context for filtering (LocalPlayer, PlayerState, or nullptr for global).
	 * Extensions only appear in extension points with matching context.
	 * 
	 * Context matching rules:
	 * - Both nullptr (global) -> MATCH
	 * - Both valid and same object -> MATCH
	 * - Otherwise -> NO MATCH
	 */
	TWeakObjectPtr<UObject> ContextObject;
	
	/** Widget class to instantiate at extension point */
	TSubclassOf<UUserWidget> WidgetClass;

	uint32 HandleID{0};
	
	/**
	 * Sorting operator for priority ordering.
	 * Higher priority extensions appear first.
	 * 
	 * Note: Lyra stores priority but never sorts - this fixes that bug!
	 */
	bool operator<(const FMCore_UIExtension& Other) const
	{
		return Priority > Other.Priority;  // Higher priority first
	}
};

/**
 * Internal data structure representing a UI extension point.
 * Extension points are slots in widgets that can receive injected extensions.
 * 
 * Not exposed to Blueprint - managed by subsystem.
 */
struct FMCore_UIExtensionPoint
{
	/** GameplayTag identifying this extension point */
	FGameplayTag ExtensionPointTag;
	
	/**
	 * Optional context for filtering (LocalPlayer, PlayerState, or nullptr for global).
	 * Only extensions with matching context will be delivered to this point.
	 * 
	 * Context matching rules:
	 * - Both nullptr (global) -> MATCH
	 * - Both valid and same object -> MATCH
	 * - Otherwise -> NO MATCH
	 */
	TWeakObjectPtr<UObject> ContextObject;
	
	/**
	 * Callback invoked when extensions are added/removed.
	 * Receives action (Added/Removed) and extension handle.
	 */
	FOnUIExtensionChanged OnChangeCallback;

	/**
	 * Check if an extension passes this extension point's contract.
	 * Validates context matching.
	 * 
	 * @param Extension - Extension to validate
	 * @return true if extension should be delivered to this point
	 */
	bool DoesExtensionPassContract(const FMCore_UIExtension* Extension) const;
};