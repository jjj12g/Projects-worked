// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_SGameMode.h"
#include "Project_SPlayerController.h"
#include "Project_SCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_SGameMode::AProject_SGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AProject_SPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}