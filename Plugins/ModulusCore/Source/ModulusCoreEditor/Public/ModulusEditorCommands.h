// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

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

    // TCommands<> interface
    virtual void RegisterCommands() override;
	
    /** Open Modulus Hub command */
    TSharedPtr<FUICommandInfo> OpenModulusHub;
};