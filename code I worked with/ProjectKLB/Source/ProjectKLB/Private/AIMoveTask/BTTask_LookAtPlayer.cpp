// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMoveTask/BTTask_LookAtPlayer.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectKLBCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Enemy.h"



UBTTask_LookAtPlayer::UBTTask_LookAtPlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "LookAtPlayer";
}

EBTNodeResult::Type UBTTask_LookAtPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    AEnemy* EnemyCharacter = Cast<AEnemy>(AIController->GetPawn());
    if (!EnemyCharacter)
    {
        return EBTNodeResult::Failed;
    }

    AProjectKLBCharacter* TargetPlayer = Cast<AProjectKLBCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject("TargetPlayer"));
    if (!TargetPlayer)
    {
        return EBTNodeResult::Failed;
    }

    // 적이 플레이어를 향해 회전
    Direction = (TargetPlayer->GetActorLocation() - EnemyCharacter->GetActorLocation()).GetSafeNormal();
    LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
    LookAtRotation.Pitch = 0.f;
    LookAtRotation.Roll = 0.f;

    // 즉시 회전 (회전 속도를 조절하려면 RInterpTo 사용 가능)
    /*FRotator CurrentRotation = EnemyCharacter->GetActorRotation();
    FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, GetWorld()->DeltaTimeSeconds, 0.5f);
    EnemyCharacter->SetActorRotation(SmoothRotation);*/
    EnemyCharacter->SetActorRotation(LookAtRotation);

    UE_LOG(LogTemp, Log, TEXT("Enemy rotated to look at player: %s"), *TargetPlayer->GetName());

    return EBTNodeResult::Succeeded;
}

void UBTTask_LookAtPlayer::RotateToTarget(AActor* Enemy, AActor* Target, float DeltaTime)
{
    if (!Enemy || !Target)
    {
        return;
    }

    Direction = (Target->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal();
    LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

    LookAtRotation.Pitch = 0.f;
    LookAtRotation.Roll = 0.f;

    // 부드러운 회전 적용
    FRotator SmoothRotation = FMath::RInterpTo(Enemy->GetActorRotation(), LookAtRotation, DeltaTime, 5.0f);
    Enemy->SetActorRotation(SmoothRotation);
}
