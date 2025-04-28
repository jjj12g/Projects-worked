// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateTargetPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKLB_API UBTService_UpdateTargetPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	

public:
	UBTService_UpdateTargetPlayer();

protected:
	// ���񽺰� Tick �� ������ ȣ��
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
