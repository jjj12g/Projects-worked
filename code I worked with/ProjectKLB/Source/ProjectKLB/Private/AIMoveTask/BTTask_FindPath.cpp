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

    // 경로 계산
    TArray<ATile*> Path = CalculatePath(AIController->GetPawn(), Cast<AActor>(Blackboard->GetValueAsObject("Target")));
    if (Path.Num() == 0)
    {
        return EBTNodeResult::Failed;
    }

    // AIController에 경로 저장
    AIController->SetPath(Path);

    // 블랙보드에 첫 번째 타일 저장
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

    // 블랙보드에서 이미 갱신된 TargetPlayer를 가져옵니다.
    UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
    AActor* TargetPlayer = BBComp ? Cast<AActor>(BBComp->GetValueAsObject("TargetPlayer")) : nullptr;
    if (!TargetPlayer)
    {
        UE_LOG(LogTemp, Warning, TEXT("No TargetPlayer set in Blackboard!"));
        return Path;
    }

    // GridManager 가져오기
    GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
    if (!GridManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("GridManager not found!"));
        return Path;
    }

    // 시작 타일과 목표 타일 가져오기
    ATile* StartTile = GridManager->GetTileAtLocation(Enemy->GetActorLocation());
    ATile* TargetTile = GridManager->GetTileAtLocation(TargetPlayer->GetActorLocation());

    if (!StartTile || !TargetTile)
    {
        UE_LOG(LogTemp, Warning, TEXT("Start or Target Tile is null!"));
        return Path;
    }

    // A* 알고리즘 실행
    Path = GridManager->FindPath(StartTile, TargetTile);
    if (Path.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No path found!"));
        return Path;
    }

    // 유효 경로 필터링 
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
