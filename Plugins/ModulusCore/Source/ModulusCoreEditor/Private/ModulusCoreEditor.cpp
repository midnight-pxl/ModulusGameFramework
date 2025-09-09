// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "ModulusCoreEditor.h"
#include "Editor/Blutility/Public/EditorUtilitySubsystem.h"
#include "CoreEditorLogging/LogModulusEditor.h"
#include "ModulusEditorCommands.h"

#define LOCTEXT_NAMESPACE "FModulusCoreEditorModule"
#define DEFINE_LOG_CATEGORY(LogModulusEditor);

void FModulusCoreEditorModule::StartupModule()
{
	FModulusEditorCommands::Register();

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FModulusCoreEditorModule::RegisterHubMenus));

	UE_LOG(LogModulusEditor, Log, TEXT("ModulusCoreEditor module started"));    
}

void FModulusCoreEditorModule::ShutdownModule()
{
	UnregisterHubMenus();

	FModulusCoreEditorModule::UnregisterHubMenus();

	UE_LOG(LogModulusEditor, Log, TEXT("ModulusCoreEditor module shutdown"));    
}

void FModulusCoreEditorModule::RegisterHubMenus()
{
	UToolMenu* MainMenu = UToolMenus::Get()->FindMenu("LevelEditor.MainMenu");
	if (MainMenu)
	{
		FToolMenuSection& ModulusSection = MainMenu->AddSection("Modulus",
			LOCTEXT("ModulusMenu", "Modulus"));

		ModulusSection.AddMenuEntry(
			"OpenModulusHub",
			LOCTEXT("OpenModulusHub", "Modulus Game Framework"),
			LOCTEXT("OpenModulusHubTooltip", "Open Modulus Framework tools and settings"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateStatic(&FModulusCoreEditorModule::OpenModulusHub))
			);
	}
}

void FModulusCoreEditorModule::UnregisterHubMenus()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
}

void FModulusCoreEditorModule::OpenModulusHub()
{
	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	if (EditorUtilitySubsystem)
	{
		UEditorUtilityWidgetBlueprint* WidgetBlueprint = LoadObject<UEditorUtilityWidgetBlueprint>(
			nullptr, TEXT("/ModulusCore/Editor/EUW_ModulusHub.EUW_ModulusHub"));

		if (WidgetBlueprint)
		{
			EditorUtilitySubsystem->SpawnAndRegisterTab(WidgetBlueprint);
		}
		else
		{
			UE_LOG(LogModulusEditor, Warning, TEXT("Could not find Modulus Hub widget blueprint"))
		}
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FModulusCoreEditorModule, ModulusCoreEditor)