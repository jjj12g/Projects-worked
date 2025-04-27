// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_EndTurn.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKLB_API UBTTask_EndTurn : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	

public:

	explicit UBTTask_EndTurn(FObjectInitializer const& ObjectInitializer);


	UBlackboardComponent* Blackboard;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;



};
