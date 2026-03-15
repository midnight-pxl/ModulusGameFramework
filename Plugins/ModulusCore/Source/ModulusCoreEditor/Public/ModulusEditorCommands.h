// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

/**
 * ModulusEditorCommands.h
 *
 * UI command definitions for the Modulus editor integration.
 */

#pragma once

#include "CoreMinimal.h"
#include "EditorStyleSet.h"
#include "Framework/Commands/Commands.h"

class FModulusEditorCommands : public TCommands<FModulusEditorCommands>
{
public:
    FModulusEditorCommands() : TCommands<FModulusEditorCommands>(
            TEXT("ModulusEditor"),
            NSLOCTEXT("Contexts", "ModulusEditor", "Modulus Editor"),
            NAME_None,
            FAppStyle::GetAppStyleSetName())
    {
    }

    virtual void RegisterCommands() override;

    TSharedPtr<FUICommandInfo> OpenModulusHub;
};