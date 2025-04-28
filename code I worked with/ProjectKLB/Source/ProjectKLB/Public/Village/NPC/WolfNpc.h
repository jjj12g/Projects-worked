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


	// NPC �ڽ��� ��縦 �����ϴ� �Լ� �������̵�
	virtual FText GetDialogueForPlayer(class AVillageCharacter* PlayerCharacter) const override;

	virtual void InitializeStats() override;


};
