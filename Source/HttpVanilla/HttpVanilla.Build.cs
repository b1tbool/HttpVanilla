using UnrealBuildTool;

public class HttpVanilla : ModuleRules
{
	public HttpVanilla(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Json", "HttpPhantom" });
	}
}