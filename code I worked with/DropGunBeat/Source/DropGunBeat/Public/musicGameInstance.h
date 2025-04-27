// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "musicGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DROPGUNBEAT_API UmusicGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	//스테이지 클리어 후 정산시 하이스코어를 비교하기 위해 사용
	UPROPERTY(BlueprintReadOnly)
	bool bIsPlaingBBKK = false;
	//스테이지중에 증가하고, 정산할때 0으로 바꿔줄 것.
	int32 currentScore = 0;

	int32 bestBBKKScore;

	int32 bestNightScore;

	bool bIsWeapon = false;
};
