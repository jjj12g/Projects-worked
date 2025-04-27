// Fill out your copyright notice in the Description page of Project Settings.


#include "Village/VillageController.h"



AVillageController::AVillageController()
{
	PrimaryActorTick.bCanEverTick = true;



}

void AVillageController::BeginPlay()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
}

void AVillageController::Tick(float DeltaTime)
{


}

void AVillageController::GameOnlyInputMode()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
	UE_LOG(LogTemp, Warning, TEXT("GameOnly"));
}

void AVillageController::GameAndUIInputMode()
{
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = true;
	UE_LOG(LogTemp, Warning, TEXT("GameAndUI"));
}




