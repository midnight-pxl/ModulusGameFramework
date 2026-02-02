// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

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
class UMCore_GameMenuHub;
class UMCore_PrimaryGameLayout;
class UTexture2D;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThemeChanged, UMCore_PDA_UITheme_Base*, NewTheme);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrimaryGameLayoutReady, UMCore_PrimaryGameLayout*, Layout);


/**
 * Central access point for ModulusCore UI systems.
 *
 * Provides:
 * - Layer stack access via gameplay tags
 * - MenuHub registration for plugin screens
 * - Theme management
 *
 * One instance per LocalPlayer (supports split-screen).
 *
 * Usage:
 *   UMCore_UISubsystem* UI = LocalPlayer->GetSubsystem<UMCore_UISubsystem>();
 *   UI->PushWidgetToLayer(MenuClass, MCore_UISettingsTags::UI_Layers_Menu);
 *   UI->GetLayerStack(MCore_UISettingsTags::UI_Layers_Modal)->AddWidget(DialogClass);
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

	/** Get the primary game layout widget for this local player */
	UFUNCTION(BlueprintPure, Category = "UI|Layout")
	UMCore_PrimaryGameLayout* GetPrimaryGameLayout() const;

	/** Returns whether a valid PrimaryGameLayout is registered */
	UFUNCTION(BlueprintPure, Category = "UI|Layout")
	bool HasPrimaryGameLayout() const { return IsValid(PrimaryGameLayout); }
	
	UPROPERTY(BlueprintAssignable, Category = "Modulus|UI|Events")
	FOnPrimaryGameLayoutReady OnPrimaryGameLayoutReady;
	
	/**
	 * Layer Stacks Access
	 */
	
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
	
	/**
	 * Menu Hub
	 */
	
	/** 
	 * Get or create the menu hub widget
	 * Caller is responsible for pushing to appropriate stack
	 * 
	 * @return MenuHub widget (created on first call, cached for reuse)
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|MenuHub")
	UMCore_GameMenuHub* GetOrCreateMenuHub();

	/** Menu screen registration */
	UFUNCTION(BlueprintCallable, Category = "UI|MenuHub")
	void RegisterMenuScreen(
		FGameplayTag TabID,
		TSubclassOf<UCommonActivatableWidget> ScreenWidgetClass,
		int32 Priority = 100,
		UTexture2D* TabIcon = nullptr);
	
	/**
	 * Unregister a menu screen tab.
	 * 
	 * @param TabID Gameplay tag of the tab to remove
	 * @return True if tab was found and removed
	 */
	UFUNCTION(BlueprintCallable, Category = "UI|MenuHub")
	bool UnregisterMenuScreen(FGameplayTag TabID);
	
	UFUNCTION(BlueprintPure, Category = "UI|MenuHub")
	const TArray<FMCore_MenuTab>& GetRegisteredMenuScreens() const
	{
		return RegisteredMenuScreens;
	}

	/**
	 * Force rebuild of MenuHub tab bar.
	 * Called automatically when screens registered/unregistered while hub is active.
	 */
	UFUNCTION(BlueprintCallable, Category = "ModulusUI|MenuHub")
	void RebuildMenuHubTabBar();
	
	/**
	 * Modulus Theme System
	 */
	
	/** Get active UI theme from Dev Settings */
	UFUNCTION(BlueprintPure, Category = "UI|Theme")
	UMCore_PDA_UITheme_Base* GetActiveTheme() const { return CachedActiveTheme; }
	
	UPROPERTY(BlueprintAssignable, Category = "UI|Theme")
	FOnThemeChanged OnThemeChanged;

	UFUNCTION(BlueprintPure, Category = "UI|Theme")
	const TArray<FMCore_ThemeEntry>& GetAvailableThemes() const;

	UFUNCTION(BlueprintPure, Category = "UI|Theme")
	int32 GetActiveThemeIndex() const { return ActiveThemeIndex; }

	UFUNCTION(BlueprintCallable, Category = "UI|Theme")
	bool SetActiveThemeByIndex(int32 ThemeIndex);

protected:
	/** Widget class for PrimaryGameLayout. Set in project defaults or override in Blueprint. */
	UPROPERTY(EditDefaultsOnly, Category = "Modulus|UI")
	TSubclassOf<UMCore_PrimaryGameLayout> PrimaryGameLayoutClass;
	
	/** Widget class for MenuHub (loaded from settings or defaults) */
	UPROPERTY()
	TSubclassOf<UMCore_GameMenuHub> MenuHubClass;

	/** Z-Order for layout when added to viewport. */
	UPROPERTY(EditDefaultsOnly, Category = "Modulus|UI", meta = (ClampMin = "-100", ClampMax = "100"))
	int32 PrimaryGameLayoutZOrder = 0;
	
	/**
	 * Called after PrimaryGameLayout is successfully created and added to viewport.
	 * Override in C++ or Blueprint subclasses for custom initialization.
	 * 
	 * @param Layout The newly created layout widget
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Modulus|UI")
	void OnPrimaryGameLayoutCreated(UMCore_PrimaryGameLayout* Layout);
	virtual void OnPrimaryGameLayoutCreated_Implementation(UMCore_PrimaryGameLayout* Layout);

private:
	void LoadWidgetClasses();
	void BuildLayerStackMap();

	/** Creates and adds PrimaryGameLayout to viewport. Called from Initialize(). */
	void CreatePrimaryGameLayout();
	/** Creates the deferred PrimaryGameLayout once PlayerController is ready (avoid race condition) */
	void OnPlayerControllerReady(APlayerController* PlayerController);
	
	FDelegateHandle PlayerControllerReadyHandle;
	
	/**
	 * Cached References
	 */
	
	/** Strong reference - UISubsystem owns the layout lifecycle */
	UPROPERTY(Transient)
	TObjectPtr<UMCore_PrimaryGameLayout> PrimaryGameLayout;
	
	UPROPERTY(Transient)
	TObjectPtr<UMCore_GameMenuHub> CachedMenuHub;
	
	UPROPERTY(Transient)
	TObjectPtr<UMCore_PDA_UITheme_Base> CachedActiveTheme;

	int32 ActiveThemeIndex{INDEX_NONE};
	
	/**
	 * Layer Management
	 */

	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetStack>> LayerStackMap;
	
	/**
	 * Hub Menu Data
	 */
	
	/** Registered menu screens for this local player */
	UPROPERTY(Transient)
	TArray<FMCore_MenuTab> RegisteredMenuScreens;
};
