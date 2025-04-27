// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKLB_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	// 예제로 저장된 레벨 이름을 저장합니다.
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FString SavedLevelName;

};
