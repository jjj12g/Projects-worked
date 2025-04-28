// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMoveTask/BTTask_FindPath.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tile/GridManager.h"
#include "Tile/Tile.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectKLBCharacter.h"



UBTTask_FindPath::UBTTask_FindPath(FObjectInitializer const& ObjectInitializer)
{
    NodeName = "Find Path";
}

EBTNodeResult::Type UBTTask_FindPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    Blackboard = AIController->GetBlackboardComponent();
    if (!Blackboard)
    {
        return EBTNodeResult::Failed;
    }

    // ��� ���
    TArray<ATile*> Path = CalculatePath(AIController->GetPawn(), Cast<AActor>(Blackboard->GetValueAsObject("Target")));
    if (Path.Num() == 0)
    {
        return EBTNodeResult::Failed;
    }

    // AIController�� ��� ����
    AIController->SetPath(Path);

    // �����忡 ù ��° Ÿ�� ����
    Blackboard->SetValueAsObject("NextTile", Path[0]);

    return EBTNodeResult::Succeeded;
}

TArray<ATile*> UBTTask_FindPath::CalculatePath(AActor* Enemy, AActor* Player)
{
    TArray<ATile*> Path;
    TArray<ATile*> ValidPath;

    APawn* EnemyPawn = AIController->GetPawn();
    if (!EnemyPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyPawn is null!"));
        return Path;
    }

    // �����忡�� �̹� ���ŵ� TargetPlayer�� �����ɴϴ�.
    UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
    AActor* TargetPlayer = BBComp ? Cast<AActor>(BBComp->GetValueAsObject("TargetPlayer")) : nullptr;
    if (!TargetPlayer)
    {
        UE_LOG(LogTemp, Warning, TEXT("No TargetPlayer set in Blackboard!"));
        return Path;
    }

    // GridManager ��������
    GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
    if (!GridManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("GridManager not found!"));
        return Path;
    }

    // ���� Ÿ�ϰ� ��ǥ Ÿ�� ��������
    ATile* StartTile = GridManager->GetTileAtLocation(Enemy->GetActorLocation());
    ATile* TargetTile = GridManager->GetTileAtLocation(TargetPlayer->GetActorLocation());

    if (!StartTile || !TargetTile)
    {
        UE_LOG(LogTemp, Warning, TEXT("Start or Target Tile is null!"));
        return Path;
    }

    // A* �˰��� ����
    Path = GridManager->FindPath(StartTile, TargetTile);
    if (Path.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No path found!"));
        return Path;
    }

    // ��ȿ ��� ���͸� 
    for (ATile* Tile : Path)
    {
        if (Tile == TargetTile || !Tile->OccupyingEnemy)
        {
            ValidPath.Add(Tile);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Tile Q=%d, R=%d is occupied, skipping."), Tile->Q, Tile->R);
        }
    }

    return ValidPath;
}
