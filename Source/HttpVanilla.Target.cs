using UnrealBuildTool;

public class HttpVanillaTarget : TargetRules
{
	public HttpVanillaTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("HttpVanilla");
	}
}