// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "ModulusCoreEditor.h"
#include "SModulusHubTab.h"
#include "ModulusEditorStyle.h"
#include "CoreEditorLogging/LogModulusEditor.h"
#include "ModulusEditorCommands.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"

#define LOCTEXT_NAMESPACE "FModulusCoreEditorModule"

void FModulusCoreEditorModule::StartupModule()
{
	// Initialize Modulus editor style
	FModulusEditorStyle::Initialize();

	FModulusEditorCommands::Register();

	// Register the Modulus Hub tab spawner
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		SModulusHubTab::TabId,
		FOnSpawnTab::CreateRaw(this, &FModulusCoreEditorModule::SpawnModulusHubTab))
		.SetDisplayName(LOCTEXT("ModulusHubTabTitle", "Modulus Hub"))
		.SetTooltipText(LOCTEXT("ModulusHubTabTooltip", "Open the Modulus Game Framework Hub"))
		.SetGroup(WorkspaceMenu::GetMenuStructure().GetToolsCategory())
		.SetIcon(FSlateIcon(FModulusEditorStyle::GetStyleSetName(), FModulusEditorStyle::ModulusIconName));

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FModulusCoreEditorModule::RegisterToolbarButton));

	UE_LOG(LogModulusEditor, Log, TEXT("ModulusCoreEditor module started"));
}

void FModulusCoreEditorModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SModulusHubTab::TabId);
	UnregisterToolbarButton();

	// Shutdown Modulus editor style
	FModulusEditorStyle::Shutdown();

	UE_LOG(LogModulusEditor, Log, TEXT("ModulusCoreEditor module shutdown"));
}

TSharedRef<SDockTab> FModulusCoreEditorModule::SpawnModulusHubTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(NomadTab)
		[
			SNew(SModulusHubTab)
		];
}

void FModulusCoreEditorModule::RegisterToolbarButton()
{
	UToolMenu* ToolBar = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	if (ToolBar)
	{
		FToolMenuSection& Section = ToolBar->FindOrAddSection("Modulus");

		Section.AddEntry(FToolMenuEntry::InitToolBarButton(
			"OpenModulusHub",
			FUIAction(FExecuteAction::CreateStatic(&FModulusCoreEditorModule::OpenModulusHub)),
			LOCTEXT("ModulusHub", "Modulus"),
			LOCTEXT("ModulusHubTooltip", "Open Modulus Game Framework Hub"),
			FSlateIcon(FModulusEditorStyle::GetStyleSetName(), FModulusEditorStyle::ModulusIconName)
		));

		UE_LOG(LogModulusEditor, Log, TEXT("Modulus toolbar button registered"));
	}
	else
	{
		UE_LOG(LogModulusEditor, Warning, TEXT("Failed to extend LevelEditor toolbar"));
	}
}

void FModulusCoreEditorModule::UnregisterToolbarButton()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
}

void FModulusCoreEditorModule::OpenModulusHub()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SModulusHubTab::TabId);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FModulusCoreEditorModule, ModulusCoreEditor)