using UnrealBuildTool;
using System.Collections.Generic;

public class proyectoOmegaGameTarget : TargetRules
{
    public proyectoOmegaGameTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.AddRange(new string[] { "proyectoOmegaGame" });
    }
}
