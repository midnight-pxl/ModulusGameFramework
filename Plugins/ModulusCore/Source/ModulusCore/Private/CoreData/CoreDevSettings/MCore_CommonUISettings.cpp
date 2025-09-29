// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "CoreData/CoreDevSettings/MCore_CommonUISettings.h"
#include "CoreData/CoreGameplayTags/MCore_UILayerTags.h"

UMCore_CommonUISettings::UMCore_CommonUISettings()
{
	GameLayerTag = MCore_UILayerTags::UI_Layer_Game;
	GameMenuLayerTag = MCore_UILayerTags::UI_Layer_GameMenu;
	MenuLayerTag = MCore_UILayerTags::UI_Layer_Menu;
	ModalLayerTag = MCore_UILayerTags::UI_Layer_Modal;
}
