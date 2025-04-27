// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PlayerAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKLB_API UBTTask_PlayerAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	

public:

	explicit UBTTask_PlayerAttack(FObjectInitializer const& ObjectInitializer);

	UBlackboardComponent* Blackboard;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	

};
