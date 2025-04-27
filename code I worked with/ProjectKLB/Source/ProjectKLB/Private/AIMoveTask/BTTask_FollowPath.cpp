// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMoveTask/BTTask_FollowPath.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tile/Tile.h"
#include "Navigation/PathFollowingComponent.h"
#include "Enemy/Enemy.h"



UBTTask_FollowPath::UBTTask_FollowPath()
{
    NodeName = "Follow Path";
}

EBTNodeResult::Type UBTTask_FollowPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("AIController is null!"));
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
    ATile* NextTile = Cast<ATile>(Blackboard->GetValueAsObject("NextTile"));

    AEnemy* EnemyCharacter = Cast<AEnemy>(AIController->GetPawn());

    if (!NextTile || NextTile->OccupyingEnemy != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("NextTile is invalid or occupied! Finding alternative."));

       
        if (EnemyCharacter)
        {
            NextTile = EnemyCharacter->FindAlternativeTile(NextTile);
            if (!NextTile)
            {
                UE_LOG(LogTemp, Warning, TEXT("No alternative tile found."));
                return EBTNodeResult::Failed;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("EnemyCharacter is null."));
            return EBTNodeResult::Failed;
        }
    }

    EnemyCharacter->MoveToTile(NextTile);

    AIController->AdvancePath();
    Blackboard->SetValueAsObject("NextTile", AIController->GetNextTile());

    return EBTNodeResult::Succeeded;
}