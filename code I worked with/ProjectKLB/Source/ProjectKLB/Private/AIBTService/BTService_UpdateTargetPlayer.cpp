// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBTService/BTService_UpdateTargetPlayer.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"        
#include "ProjectKLBCharacter.h"
#include "Enemy/Enemy.h"


UBTService_UpdateTargetPlayer::UBTService_UpdateTargetPlayer()
{
    // 서비스의 이름 설정
    NodeName = "Update Target Player";

    // 서비스가 주기적으로 실행될 간격과 무작위 변동값 설정 
    Interval = 1.0f;
    RandomDeviation = 0.2f;
}

void UBTService_UpdateTargetPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        return;
    }

    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return;
    }

    APawn* EnemyPawn = AIController->GetPawn();
    if (!EnemyPawn)
    {
        return;
    }

    // 가장 가까운 플레이어 찾기
    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AProjectKLBCharacter::StaticClass(), PlayerActors);
    AProjectKLBCharacter* ClosestPlayer = nullptr;
    float ClosestDistanceSq = FLT_MAX;
    for (AActor* Actor : PlayerActors)
    {
        float DistanceSq = FVector::DistSquared(Actor->GetActorLocation(), EnemyPawn->GetActorLocation());
        if (DistanceSq < ClosestDistanceSq)
        {
            ClosestDistanceSq = DistanceSq;
            ClosestPlayer = Cast<AProjectKLBCharacter>(Actor);
        }
    }

    if (ClosestPlayer)
    {
        BlackboardComp->SetValueAsObject("TargetPlayer", ClosestPlayer);
        //UE_LOG(LogTemp, Log, TEXT("BTService_UpdateTargetPlayer: Updated target to %s"), *ClosestPlayer->GetName());

        // 적의 내부 TargetPlayer도 업데이트
        AEnemy* EnemyCharacter = Cast<AEnemy>(EnemyPawn);
        if (EnemyCharacter)
        {
            EnemyCharacter->SetTargetPlayer(ClosestPlayer);
        }
    }
}
