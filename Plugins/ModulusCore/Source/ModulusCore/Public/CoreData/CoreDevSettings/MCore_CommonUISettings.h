// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CoreData/CoreStructEnums/SettingsStructsEnums/CoreGameSettings/MCore_GameSettingType.h"
#include "CoreData/CoreDataAssets/UIDAs/MCore_DA_UITheme_Base.h"
#include "Engine/DeveloperSettings.h"
#include "MCore_CommonUISettings.generated.h"

/**
 * 
 */
UCLASS(Config=game, DefaultConfig, meta=(DisplayName="Modulus UI Settings"))
class MODULUSCORE_API UMCore_CommonUISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UMCore_CommonUISettings() = default;

	UPROPERTY(config, EditAnywhere, Category = "Save System")
	FString DefaultSaveSlotName = TEXT("GameSettings");
	
	// Tag-based settings caching
	UPROPERTY(config, EditAnywhere, Category = "Performance")
	bool bEnableTagCaching{true};

	UPROPERTY(Config, EditAnywhere, Category = "Configuration",
		meta=(AllowedClasses = "MCore_UIThemeDataAsset_Base"))
	TSoftObjectPtr<UMCore_DA_UITheme_Base> CurrentThemeAsset;

	UPROPERTY(Config, EditAnywhere, Category = "WidgetClasses")
	TMap<EMCore_SettingType, TSoftClassPtr<UUserWidget>> WidgetClassOverrides;

	// Current DataAsset for Default Settings values
	UPROPERTY(Config, EditAnywhere, Category = "Configuration",
		meta=(DisplayName = "Default Settings Config"))
	TSoftObjectPtr<class UMCore_DA_GameSettings_Organization> CurrentDefaultSettings;

	static const UMCore_CommonUISettings* Get();

	const UMCore_DA_UITheme_Base* GetCurrentTheme() const { return CurrentThemeAsset.Get(); }
};
