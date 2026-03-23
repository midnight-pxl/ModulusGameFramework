// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * MCore_UISubsystem.h
 *
 * Per-player UI subsystem managing widget lifecycle, layer stacks,
 * menu hub orchestration, and theme distribution.
 */

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "GameplayTagContainer.h"
#include "CoreUI/Widgets/MCore_PrimaryGameLayout.h"
#include "CoreData/Types/UI/MCore_MenuTabTypes.h"
#include "CoreData/Types/UI/MCore_ThemeTypes.h"
#include "MCore_UISubsystem.generated.h"

class UCommonActivatableWidgetStack;
class UCommonActivatableWidget;
class UMCore_PDA_UITheme_Base;
class UMCore_PlayerSettingsSave;
class UMCore_GameMenuHub;
class UMCore_PrimaryGameLayout;
class UTexture2D;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThemeChanged, UMCore_PDA_UITheme_Base*, NewTheme);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrimaryGameLayoutReady, UMCore_PrimaryGameLayout*, Layout);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWidgetLayerChanged, UCommonActivatableWidget*, Widget, FGameplayTag, LayerTag);


/**
 * Central access point for ModulusCore UI systems. One instance per LocalPlayer (split-screen safe).
 *
 * Manages PrimaryGameLayout lifecycle, 4-layer widget stack access via gameplay tags,
 * theme distribution, menu hub orchestration, and player settings caching.
 */
UCLASS()
class MODULUSCORE_API UMCore_UISubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	//~ Begin USubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~ End USubsystem Interface

	UFUNCTION(BlueprintPure, Category = "UI|Layout")
	UMCore_PrimaryGameLayout* GetPrimaryGameLayout() const;

	UFUNCTION(BlueprintPure, Category = "UI|Layout")
	bool HasPrimaryGameLayout() const { return IsValid(PrimaryGameLayout); }
	
	UPROPERTY(BlueprintAssignable, Category = "Modulus|UI|Events")
	FOnPrimaryGameLayoutReady OnPrimaryGameLayoutReady;
	
// ============================================================================
// LAYER STACKS
// ============================================================================

	/** Get stack by layer tag. Returns nullptr if layout not ready or tag invalid. */
	UFUNCTION(BlueprintPure, Category = "Modulus|UI|Layout")
	UCommonActivatableWidgetStack* GetLayerStack(FGameplayTag LayerTag) const;

	/** Push widget to layer by tag. Returns created widget or nullptr. */
	UFUNCTION(BlueprintCallable, Category = "Modulus|UI|Layout", meta = (DeterminesOutputType = "WidgetClass"))
	UCommonActivatableWidget* PushWidgetToLayer(TSubclassOf<UCommonActivatableWidget> WidgetClass,
		UPARAM(meta = (Categories = "MCore.UI.Layer")) FGameplayTag LayerTag);

	/** Check if a layer has any active widget. */
	UFUNCTION(BlueprintPure, Category = "Modulus|UI|Layout")
	bool IsLayerActive(FGameplayTag LayerTag) const;

	UPROPERTY(BlueprintAssignable, Category = "Modulus|UI|Events")
	FOnWidgetLayerChanged OnWidgetPushed;

	UPROPERTY(BlueprintAssignable, Category = "Modulus|UI|Events")
	FOnWidgetLayerChanged OnWidgetRemoved;

	void NotifyWidgetDestroyed(UCommonActivatableWidget* Widget);

// ============================================================================
// PLAYER SETTINGS
// ============================================================================

	/** Get cached player settings. Loads from disk on first access. */
	UFUNCTION(BlueprintCallable, Category = "ModulusCore|Settings")
	UMCore_PlayerSettingsSave* GetPlayerSettings();
	
// ============================================================================
// MENU HUB
// ============================================================================

	/** Returns the menu hub widget, creating it on first call. Caller pushes to appropriate stack. */
	UFUNCTION(BlueprintCallable, Category = "UI|MenuHub")
	UMCore_GameMenuHub* GetOrCreateMenuHub();

	/** Register a menu screen tab in the MenuHub. Duplicate TabIDs are rejected. */
	UFUNCTION(BlueprintCallable, Category = "UI|MenuHub")
	void RegisterMenuScreen(
		FGameplayTag TabID,
		TSubclassOf<UCommonActivatableWidget> ScreenWidgetClass,
		int32 Priority = 100,
		UTexture2D* TabIcon = nullptr);
	
	/** Unregister a menu screen tab. Returns true if found and removed. */
	UFUNCTION(BlueprintCallable, Category = "UI|MenuHub")
	bool UnregisterMenuScreen(FGameplayTag TabID);
	
	UFUNCTION(BlueprintPure, Category = "UI|MenuHub")
	const TArray<FMCore_MenuTab>& GetRegisteredMenuScreens() const { return RegisteredMenuScreens; }

	/**
	 * Force rebuild of MenuHub tab bar.
	 * Called automatically when screens registered/unregistered while hub is active.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusUI|MenuHub")
	void RebuildMenuHubTabBar();
	
// ============================================================================
// THEME
// ============================================================================

	UFUNCTION(BlueprintPure, Category = "UI|Theme")
	UMCore_PDA_UITheme_Base* GetActiveTheme() const { return CachedActiveTheme; }
	
	UPROPERTY(BlueprintAssignable, Category = "UI|Theme")
	FOnThemeChanged OnThemeChanged;

	UFUNCTION(BlueprintPure, Category = "UI|Theme")
	const TArray<FMCore_ThemeEntry>& GetAvailableThemes() const;

	UFUNCTION(BlueprintPure, Category = "UI|Theme")
	int32 GetActiveThemeIndex() const { return ActiveThemeIndex; }

	/** Returns the active text size index from the Accessibility.UITextSize setting (clamped >= 0). */
	UFUNCTION(BlueprintPure, Category = "Modulus|Theme")
	int32 GetActiveTextSizeIndex() const;

	UFUNCTION(BlueprintCallable, Category = "UI|Theme")
	bool SetActiveThemeByIndex(int32 ThemeIndex);

protected:
	/* Widget class for PrimaryGameLayout. Set in project defaults or override in Blueprint. */
	UPROPERTY(EditDefaultsOnly, Category = "Modulus|UI")
	TSubclassOf<UMCore_PrimaryGameLayout> PrimaryGameLayoutClass;
	
	/* Widget class for MenuHub (loaded from settings or defaults) */
	UPROPERTY()
	TSubclassOf<UMCore_GameMenuHub> MenuHubClass;

	/* Z-order for layout when added to viewport */
	UPROPERTY(EditDefaultsOnly, Category = "Modulus|UI", meta = (ClampMin = "-100", ClampMax = "100"))
	int32 PrimaryGameLayoutZOrder = 0;
	
	/**
	 * Called after PrimaryGameLayout is successfully created and added to viewport.
	 * Override in C++ or Blueprint subclasses for custom initialization.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Modulus|UI")
	void OnPrimaryGameLayoutCreated(UMCore_PrimaryGameLayout* Layout);
	virtual void OnPrimaryGameLayoutCreated_Implementation(UMCore_PrimaryGameLayout* Layout);

private:
	void LoadWidgetClasses();
	void BuildLayerStackMap();
	void CompactTrackedWidgets(FGameplayTag LayerTag);
	void UntrackWidget(UCommonActivatableWidget* Widget, FGameplayTag LayerTag);

	/* Creates and adds PrimaryGameLayout to viewport */
	void CreatePrimaryGameLayout();
	/* Deferred layout creation once PlayerController is ready */
	void OnPlayerControllerReady(APlayerController* PlayerController);
	
	FDelegateHandle PlayerControllerReadyHandle;
	
	/* Strong reference; UISubsystem owns the layout lifecycle */
	UPROPERTY(Transient)
	TObjectPtr<UMCore_PrimaryGameLayout> PrimaryGameLayout;
	
	UPROPERTY(Transient)
	TObjectPtr<UMCore_GameMenuHub> CachedMenuHub;
	
	UPROPERTY(Transient)
	TObjectPtr<UMCore_PDA_UITheme_Base> CachedActiveTheme;

	int32 ActiveThemeIndex{INDEX_NONE};
	
	/* Cached player settings, loaded on first access and saved on Deinitialize */
	UPROPERTY(Transient)
	TObjectPtr<UMCore_PlayerSettingsSave> CachedPlayerSettings;
	
	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetStack>> LayerStackMap;

	/* Widgets pushed via PushWidgetToLayer, tracked per-layer with weak refs */
	TMap<FGameplayTag, TArray<TWeakObjectPtr<UCommonActivatableWidget>>> TrackedWidgets;

	/* Registered menu screens for this local player */
	UPROPERTY(Transient)
	TArray<FMCore_MenuTab> RegisteredMenuScreens;
};
