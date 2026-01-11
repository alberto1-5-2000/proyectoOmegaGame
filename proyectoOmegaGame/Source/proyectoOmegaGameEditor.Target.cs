using UnrealBuildTool;
using System.Collections.Generic;

public class proyectoOmegaGameEditorTarget : TargetRules
{
    public proyectoOmegaGameEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.AddRange(new string[] { "proyectoOmegaGame" });
    }
}
