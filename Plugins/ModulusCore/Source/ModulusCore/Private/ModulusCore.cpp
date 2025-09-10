// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

#include "ModulusCore.h"

#define LOCTEXT_NAMESPACE "FModulusCoreModule"

void FModulusCoreModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FModulusCoreModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FModulusCoreModule, ModulusCore)