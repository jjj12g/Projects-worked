// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/WolfAttackAnimNotify.h"
#include "Enemy/Enemy.h"
#include "ProjectKLBCharacter.h"
#include "GameFramework/Actor.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


void UWolfAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("MeshComp is null in CustomAnimNotify"));
        return;
    }

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("Owner is null in CustomAnimNotify"));
        return;
    }

    AEnemy* EnemyCharacter = Cast<AEnemy>(Owner);
    if (EnemyCharacter)
    {
        // AI 컨트롤러를 통해 블랙보드에서 타겟을 직접 가져옵니다.
        AEnemyAIController* AIController = Cast<AEnemyAIController>(EnemyCharacter->GetController());
        if (AIController && AIController->GetBlackboardComponent())
        {
            AProjectKLBCharacter* TargetPlayer = Cast<AProjectKLBCharacter>(
                AIController->GetBlackboardComponent()->GetValueAsObject("TargetPlayer"));
            int32 DamageAmount = 10; // 데미지 값
            if (TargetPlayer)
            {
                TargetPlayer->ApplyDamage(DamageAmount);
                UE_LOG(LogTemp, Log, TEXT("Applied %d damage to %s"), DamageAmount, *TargetPlayer->GetName());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("AIController or Blackboard is null in CustomAnimNotify"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Owner is not an AEnemyCharacter in CustomAnimNotify"));
    }
}
