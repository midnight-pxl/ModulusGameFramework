// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "CoreData/Types/UI/MCore_UIExtensionTypes.h"
#include "MCore_UIExtensionSubsystem.generated.h"

class UUserWidget;

/**
 * World subsystem for UI extension point and extension management.
 * 
 * Enables plugins to inject widgets into predefined UI slots without coupling.
 * Acts as centralized registry matching extension points with extensions via GameplayTags.
 * 
 * Key Features:
 * - Tag-based pub/sub (no direct plugin coupling)
 * - Priority-based ordering (higher priority created first)
 * - Context filtering (per-LocalPlayer, global, or custom scoping)
 * - Zero per-frame cost (event-driven)
 * 
 * Common Uses:
 * - Plugin adds HUD widget to core game HUD
 * - Multiple plugins extend same menu without conflicts
 * - Per-player UI in splitscreen scenarios
 * 
 * Network Behavior: Client-only, no replication
 * 
 * @see FMCore_UIExtensionHandle, FMCore_UIExtensionPointHandle
 */
UCLASS()
class MODULUSCORE_API UMCore_UIExtensionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	 * Register extension point that receives widget injections.
	 * Callback immediately invoked for existing matching extensions (sorted by priority).
	 * 
	 * @param ExtensionPointTag - GameplayTag identifying this extension point
	 * @param ContextObject - Context for filtering (LocalPlayer for per-player, nullptr for global)
	 * @param ExtensionCallback - Invoked when extensions added/removed
	 * @return Handle for unregistration
	 */
	UFUNCTION(BlueprintCallable, Category="Modulus|UI Extension", 
		meta=(AutoCreateRefTerm="ExtensionCallback"))
	FMCore_UIExtensionPointHandle RegisterExtensionPoint(
		const FGameplayTag& ExtensionPointTag,
		UObject* ContextObject,
		const FOnUIExtensionChanged& ExtensionCallback);

	/**
	 * Unregister extension point (stop receiving extensions).
	 * Safe to call with invalid handle.
	 */
	UFUNCTION(BlueprintCallable, Category="Modulus|UI Extension")
	void UnregisterExtensionPoint(UPARAM(ref) FMCore_UIExtensionPointHandle& ExtensionPointHandle);

	/**
	 * Register extension (widget) to inject at matching extension points.
	 * Immediately notifies all matching extension points.
	 * 
	 * @param ExtensionPointTag - GameplayTag identifying target extension point(s)
	 * @param ContextObject - Context for filtering (LocalPlayer for per-player, nullptr for global)
	 * @param WidgetClass - Widget class to instantiate
	 * @param Priority - Sort order (higher created first, default: 0)
	 * @return Handle for unregistration
	 */
	UFUNCTION(BlueprintCallable, Category="Modulus|UI Extension")
	FMCore_UIExtensionHandle RegisterExtension(
		const FGameplayTag& ExtensionPointTag,
		UObject* ContextObject,
		TSubclassOf<UUserWidget> WidgetClass,
		int32 Priority = 0);

	/**
	 * Unregister extension (stop injecting into extension points).
	 * Notifies all extension points to remove widget.
	 * Safe to call with invalid handle.
	 */
	UFUNCTION(BlueprintCallable, Category="Modulus|UI Extension")
	void UnregisterExtension(UPARAM(ref) FMCore_UIExtensionHandle& ExtensionHandle);

	/**
	 * Get widget class for extension handle.
	 * Used by extension point widgets to create instances.
	 */
	UFUNCTION(BlueprintCallable, Category="Modulus|UI Extension")
	TSubclassOf<UUserWidget> GetWidgetClassForExtension(const FMCore_UIExtensionHandle& ExtensionHandle) const;

	/** Check if extension handle is currently registered */
	UFUNCTION(BlueprintPure, Category="Modulus|UI Extension")
	bool IsExtensionRegistered(const FMCore_UIExtensionHandle& ExtensionHandle) const;

	/** Check if extension point handle is currently registered */
	UFUNCTION(BlueprintPure, Category="Modulus|UI Extension")
	bool IsExtensionPointRegistered(const FMCore_UIExtensionPointHandle& ExtensionPointHandle) const;

private:
	void NotifyExtensionPointsOfExtension(EMCore_UIExtensionAction Action, const FMCore_UIExtension* Extension);
	void NotifyExtensionPointOfExtensions(const FMCore_UIExtensionPoint* ExtensionPoint);

	TMap<FGameplayTag, TArray<TSharedPtr<FMCore_UIExtensionPoint>>> ExtensionPointMap;
	TMap<FGameplayTag, TArray<TSharedPtr<FMCore_UIExtension>>> ExtensionMap;
	TMap<uint32, TSharedPtr<FMCore_UIExtensionPoint>> ExtensionPointHandles;
	TMap<uint32, TSharedPtr<FMCore_UIExtension>> ExtensionHandles;
	uint32 NextHandleID{1};
};