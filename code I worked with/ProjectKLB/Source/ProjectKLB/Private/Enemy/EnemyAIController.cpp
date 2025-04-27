// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"         // 플레이어 캐릭터 가져오기
#include "ProjectKLBCharacter.h"




AProjectKLBCharacter* AEnemyAIController::GetClosestPlayer(UWorld* World, AActor* EnemyActor)
{
    UE_LOG(LogTemp, Warning, TEXT("GetClosestPlayer ON"));
    if (!World || !EnemyActor) return nullptr;

    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsOfClass(World, AProjectKLBCharacter::StaticClass(), PlayerActors);

    AProjectKLBCharacter* ClosestPlayer = nullptr;
    float ClosestDistanceSq = FLT_MAX;
    for (AActor* Actor : PlayerActors)
    {
        float DistanceSq = FVector::DistSquared(Actor->GetActorLocation(), EnemyActor->GetActorLocation());
        if (DistanceSq < ClosestDistanceSq)
        {
            ClosestDistanceSq = DistanceSq;
            ClosestPlayer = Cast<AProjectKLBCharacter>(Actor);
            UE_LOG(LogTemp, Warning, TEXT("GetClosestPlayer Actor"));
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("GetClosestPlayer Off"));
    return ClosestPlayer;
}

// 경로 설정 함수
void AEnemyAIController::SetPath(const TArray<ATile*>& NewPath)
{
    UE_LOG(LogTemp, Warning, TEXT("AI SetPath"));
    CurrentPath = NewPath;
}

ATile* AEnemyAIController::GetNextTile()
{
    UE_LOG(LogTemp, Warning, TEXT("AI GetNextTile"));
    return CurrentPath.Num() > 0 ? CurrentPath[0] : nullptr;
}

void AEnemyAIController::AdvancePath()
{
    if (CurrentPath.Num() > 0)
    {
        ATile* NextTile = CurrentPath[0];
        CurrentPath.RemoveAt(0);

        // 에너미 Pawn을 가져와 MoveToTile 호출
        AEnemy* EnemyCharacter = Cast<AEnemy>(GetPawn());
        if (EnemyCharacter && NextTile)
        {
            EnemyCharacter->MoveToTile(NextTile);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("EnemyCharacter or NextTile is null!"));
        }

        UE_LOG(LogTemp, Log, TEXT("Advanced Path. Remaining tiles: %d"), CurrentPath.Num());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No tiles left in path to advance."));
    }
}

AEnemyAIController::AEnemyAIController(FObjectInitializer const& ObjectInitializer)
    : Super(ObjectInitializer)
{
  
  
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);


    AEnemy* const Enemy = Cast<AEnemy>(InPawn);
    if (Enemy)
    {
        if (UBehaviorTree* const Tree = Enemy->GetBehaviorTree())
        {
            BlackboardComponent = nullptr;
            UseBlackboard(Tree->BlackboardAsset, BlackboardComponent);
            Blackboard = BlackboardComponent;
            RunBehaviorTree(Tree);
        }
    }

    if (Blackboard)
    {
        // 가장 가까운 플레이어를 찾습니다.
        AProjectKLBCharacter* ClosestPlayer = AEnemyAIController::GetClosestPlayer(GetWorld(), InPawn);
        if (ClosestPlayer)
        {
            Blackboard->SetValueAsObject("TargetPlayer", ClosestPlayer);
            UE_LOG(LogTemp, Log, TEXT("Player set in Blackboard: %s"), *ClosestPlayer->GetName());
            Enemy->SetTargetPlayer(ClosestPlayer);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No closest player found!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Blackboard is not initialized in AIController."));
    }
}

void AEnemyAIController::AttackPlayer()
{
    // 공격 로직 구현
    UE_LOG(LogTemp, Log, TEXT("Enemy attacks the player!"));
}


