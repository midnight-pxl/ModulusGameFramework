// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved


#include "CoreData/CoreInterfaces/UIInterfaces/MCore_ThemeableInterface.h"
#include "CoreUISystem/CoreWidgets/WidgetPrimitives/MCore_ActivatableBase.h"
#include "CoreData/CoreDataAssets/UIDAs/MCore_DA_UITheme_Base.h"
#include "CoreData/CoreDevSettings/MCore_CommonUISettings.h"
#include "CoreData/CoreLogging/LogModulusUI.h"

void IMCore_ThemeableInterface::RefreshTheme()
{
	// Get the widget (must inherit from UMCore_ActivatableWidgetBase)
	UMCore_ActivatableBase* Widget = Cast<UMCore_ActivatableBase>(_getUObject());
	if (!Widget)
	{
		UE_LOG(LogModulusUI, Error, 
			TEXT("IMCore_Themeable::RefreshTheme - Widget must inherit from UMCore_ActivatableWidgetBase"));
		return;
	}
    
	// Get Developer Settings
	const UMCore_CommonUISettings* DevSettings = GetDefault<UMCore_CommonUISettings>();
	if (!DevSettings)
	{
		UE_LOG(LogModulusUI, Warning, 
			TEXT("IMCore_Themeable::RefreshTheme - Failed to get Developer Settings"));
		return;
	}
    
	// Get active theme
	UMCore_DA_UITheme_Base* ActiveTheme = const_cast<UMCore_DA_UITheme_Base*>(DevSettings->GetCurrentTheme());
	if (!ActiveTheme)
	{
		UE_LOG(LogModulusUI, Warning, 
			TEXT("IMCore_Themeable::RefreshTheme - No active theme configured"));
		return;
	}
    
	// Check if theme actually changed (access base class cache via friend)
	if (Widget->CachedTheme == ActiveTheme) { return; }
    
	// Update cache (write access via friend)
	Widget->CachedTheme = ActiveTheme;
    
	// Trigger theme application
	Execute_OnThemeChanged(_getUObject(), ActiveTheme);
}
