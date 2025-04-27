// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectKLB : ModuleRules
{
	public ProjectKLB(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "UMG","Slate","SlateCore", "AIModule"/*AI ���� ���*/, "GameplayTasks"/*�����̺�� Ʈ�� �½�ũ ���� ���*/, }); 



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

