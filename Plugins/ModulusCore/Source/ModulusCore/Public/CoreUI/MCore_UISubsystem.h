// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "GameplayTagContainer.h"
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
	
	/**
	 * Layout Registration
	 */
	
	/**
	 * Register primary game layout (called automatically by layout).
	 * Builds layer stack map for tag-based access.
	 */
	UFUNCTION(BlueprintCallable, Category = "Modulus|UI|Layout")
	bool RegisterPrimaryGameLayout(UMCore_PrimaryGameLayout* InLayout);

	/** Unregister primary game layout. Clears cached reference and layer map. */
	UFUNCTION(BlueprintCallable, Category = "Modulus|UI|Layout")
	void UnregisterPrimaryGameLayout();

	/** Get the primary game layout widget for this local player */
	UFUNCTION(BlueprintPure, Category = "UI|Layout")
	UMCore_PrimaryGameLayout* GetPrimaryGameLayout() const;

	/** Returns whether a valid PrimaryGameLayout is registered */
	UFUNCTION(BlueprintPure, Category = "UI|Layout")
	bool HasPrimaryGameLayout() const { return CachedPrimaryGameLayout.IsValid(); }
	
	/**
	 * Layer Access
	 */
	
	/** Get stack by layer tag. Returns nullptr if layout not ready or tag invalid. */
	UFUNCTION(BlueprintPure, Category = "Modulus|UI|Layout")
	UCommonActivatableWidgetStack* GetLayerStack(FGameplayTag LayerTag) const;

	/** Push widget to layer by tag. Returns created widget or nullptr. */
	UFUNCTION(BlueprintCallable, Category = "Modulus|UI|Layout", meta = (DeterminesOutputType = "WidgetClass"))
	UCommonActivatableWidget* PushWidgetToLayer(TSubclassOf<UCommonActivatableWidget> WidgetClass, FGameplayTag LayerTag);

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
	/** Widget class for MenuHub (loaded from settings or defaults) */
	UPROPERTY()
	TSubclassOf<UMCore_GameMenuHub> MenuHubClass;

private:
	void LoadWidgetClasses();
	void BuildLayerStackMap();

	/**
	 * Cached References
	 */
	
	UPROPERTY(Transient)
	TWeakObjectPtr<UMCore_PrimaryGameLayout> CachedPrimaryGameLayout;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<UMCore_GameMenuHub> CachedMenuHub;
	
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
