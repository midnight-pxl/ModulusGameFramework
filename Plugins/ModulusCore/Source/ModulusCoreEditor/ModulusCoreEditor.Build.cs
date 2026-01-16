// Copyright 2025, Midnight Pixel Studio LLC. All Rights Reserved

using UnrealBuildTool;

public class ModulusCoreEditor : ModuleRules
{
    public ModulusCoreEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "ModulusCore",
                "EditorStyle"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "EditorWidgets",
                "UnrealEd",
                "ToolMenus",
                "ToolWidgets",
                "Slate",
                "SlateCore",
                "EditorSubsystem",
                "UMG",
                "UMGEditor",
                "EditorScriptingUtilities",
                "Blutility",
                "Json",
                "JsonUtilities",
                "DesktopPlatform",
                "Projects"
            }
        );
    }
}