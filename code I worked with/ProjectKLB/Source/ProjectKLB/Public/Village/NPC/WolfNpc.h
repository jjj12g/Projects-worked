// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Village/NPC/FavorabilityBaseNpcCharacter.h"
#include "WolfNpc.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKLB_API AWolfNpc : public AFavorabilityBaseNpcCharacter
{
	GENERATED_BODY()
	

public:
	AWolfNpc();


	// NPC 자신의 대사를 결정하는 함수 오버라이드
	virtual FText GetDialogueForPlayer(class AVillageCharacter* PlayerCharacter) const override;

	virtual void InitializeStats() override;


};
