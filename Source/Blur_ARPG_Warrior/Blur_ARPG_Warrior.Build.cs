// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

//关于项目：
//项目用于学习制作ARPG游戏的完整流程。
//“Notes：”注释是一些知识点的记录。“Tips：”注释是关于项目的注意点或额外注释。其他注释一般用于提示代码流程或功能用法。
//“TODO：”是未完成的功能或等待完善的内容。

public class Blur_ARPG_Warrior : ModuleRules
{
	public Blur_ARPG_Warrior(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"GameplayTags",
			"EnhancedInput",
			"GameplayTasks",
			"AnimGraphRuntime",
			"MotionWarping",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
