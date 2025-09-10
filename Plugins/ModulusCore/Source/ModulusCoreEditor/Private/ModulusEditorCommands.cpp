// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "ModulusEditorCommands.h"

#define LOCTEXT_NAMESPACE "FModulusEditorCommands"

void FModulusEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenModulusHub, "Modulus Hub", "Open the Modulus Game Framework Hub", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE