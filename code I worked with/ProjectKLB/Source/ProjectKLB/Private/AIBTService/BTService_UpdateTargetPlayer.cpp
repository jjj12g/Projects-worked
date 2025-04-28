// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBTService/BTService_UpdateTargetPlayer.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"        
#include "ProjectKLBCharacter.h"
#include "Enemy/Enemy.h"


UBTService_UpdateTargetPlayer::UBTService_UpdateTargetPlayer()
{
    // ������ �̸� ����
    NodeName = "Update Target Player";

    // ���񽺰� �ֱ������� ����� ���ݰ� ������ ������ ���� 
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

    // ���� ����� �÷��̾� ã��
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

        // ���� ���� TargetPlayer�� ������Ʈ
        AEnemy* EnemyCharacter = Cast<AEnemy>(EnemyPawn);
        if (EnemyCharacter)
        {
            EnemyCharacter->SetTargetPlayer(ClosestPlayer);
        }
    }
}
