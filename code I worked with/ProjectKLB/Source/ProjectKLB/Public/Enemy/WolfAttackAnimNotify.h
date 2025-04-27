// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "WolfAttackAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKLB_API UWolfAttackAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
	// Notify ȣ�� �� ����� �Լ�
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
