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
        // �÷��̾ �ֺ��� ������ ���� ���·� ��ȯ
        Blackboard->SetValueAsBool("AttackPlayer", true);
        return EBTNodeResult::Succeeded;
    }
    

    // �÷��̾ �ֺ��� ������ �̵��ϵ��� ����
    Blackboard->SetValueAsBool("AttackPlayer", false);
    return EBTNodeResult::Failed;
}

bool UBTTask_CheckSurroundingTiles::IsPlayerInSurroundingTiles(AEnemyAIController* AIController)
{
    // ���� ��ġ�� Ÿ�� ��������
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

    // �ֺ� 6Ÿ�� �˻�
    TArray<ATile*> NeighborTiles = GridManager->GetNeighboringTiles(CurrentTile);
    for (ATile* Tile : NeighborTiles)
    {
        if (Tile && Tile->bHasPlayer) // bHasPlayer�� �÷��̾ �ش� Ÿ�Ͽ� �ִ��� ���θ� ��Ÿ��
        {
            UE_LOG(LogTemp, Warning, TEXT("CurrentTile"));
            AIController->AttackPlayer();
            return true;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("IsPlayerInSurroundingTiles false"));
    return false;
}
