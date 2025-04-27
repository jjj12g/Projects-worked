// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPath.generated.h"

/**
 * 
 */
 class ATile;
 class AEnemyAIController;
 class UBlackboardComponent;
 class AGridManager;
 class AProjectKLBPlayerController;
 class AProjectKLBCharacter;


UCLASS()
class PROJECTKLB_API UBTTask_FindPath : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	

public:

    explicit UBTTask_FindPath(FObjectInitializer const& ObjectInitializer);

    AEnemyAIController* AIController;

    UBlackboardComponent* Blackboard;

    AGridManager* GridManager;

    AProjectKLBPlayerController* PC;

    AProjectKLBCharacter* ClosestPlayer;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    // A* 알고리즘으로 경로를 계산
    TArray<ATile*> CalculatePath(AActor* Enemy, AActor* Player);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
    float SearchRadius = 1500.f;

};
