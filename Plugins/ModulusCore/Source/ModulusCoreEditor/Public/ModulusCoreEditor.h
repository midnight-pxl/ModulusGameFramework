// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class SDockTab;
class FSpawnTabArgs;

class FModulusCoreEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    static void OpenModulusHub();

private:
    TSharedRef<SDockTab> SpawnModulusHubTab(const FSpawnTabArgs& SpawnTabArgs);
    void RegisterToolbarButton();
    void UnregisterToolbarButton();
};
