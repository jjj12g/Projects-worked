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
	
	// Notify 호출 시 실행될 함수
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
