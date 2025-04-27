// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMoveTask/BTTask_CheckSurroundingTiles.h"
#include "Enemy/EnemyAIController.h"
#include "Tile/GridManager.h"
#include "Tile/Tile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTTask_CheckSurroundingTiles::UBTTask_CheckSurroundingTiles(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "CheckSurroundingTiles";
}

EBTNodeResult::Type UBTTask_CheckSurroundingTiles::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    Blackboard = AIController->GetBlackboardComponent();
    Blackboard->SetValueAsBool("AttackPlayer", false);
    if (IsPlayerInSurroundingTiles(AIController))
    {
        // 플레이어가 주변에 있으면 공격 상태로 전환
        Blackboard->SetValueAsBool("AttackPlayer", true);
        return EBTNodeResult::Succeeded;
    }
    

    // 플레이어가 주변에 없으면 이동하도록 설정
    Blackboard->SetValueAsBool("AttackPlayer", false);
    return EBTNodeResult::Failed;
}

bool UBTTask_CheckSurroundingTiles::IsPlayerInSurroundingTiles(AEnemyAIController* AIController)
{
    // 현재 위치의 타일 가져오기
    APawn* EnemyPawn = AIController->GetPawn();
    AGridManager* GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
    if (!GridManager || !EnemyPawn)
    {
        return false;
    }

    ATile* CurrentTile = GridManager->GetTileAtLocation(EnemyPawn->GetActorLocation());
    if (!CurrentTile)
    {
        return false;
    }

    // 주변 6타일 검색
    TArray<ATile*> NeighborTiles = GridManager->GetNeighboringTiles(CurrentTile);
    for (ATile* Tile : NeighborTiles)
    {
        if (Tile && Tile->bHasPlayer) // bHasPlayer는 플레이어가 해당 타일에 있는지 여부를 나타냄
        {
            UE_LOG(LogTemp, Warning, TEXT("CurrentTile"));
            AIController->AttackPlayer();
            return true;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("IsPlayerInSurroundingTiles false"));
    return false;
}
