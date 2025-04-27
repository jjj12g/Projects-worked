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
	//�������� Ŭ���� �� ����� ���̽��ھ ���ϱ� ���� ���
	UPROPERTY(BlueprintReadOnly)
	bool bIsPlaingBBKK = false;
	//���������߿� �����ϰ�, �����Ҷ� 0���� �ٲ��� ��.
	int32 currentScore = 0;

	int32 bestBBKKScore;

	int32 bestNightScore;

	bool bIsWeapon = false;
};
