// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectKLB : ModuleRules
{
	public ProjectKLB(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "UMG","Slate","SlateCore", "AIModule"/*AI 관련 모듈*/, "GameplayTasks"/*비헤이비어 트리 태스크 관련 모듈*/, }); 



        PublicIncludePaths.AddRange(
            new string[] {
        "ProjectKLB/Public"
                
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
        "ProjectKLB/Private"
               
            }
        );


    }
}

