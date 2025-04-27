// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMoveTask/BTTask_PlayerAttack.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectKLBCharacter.h"
#include "Enemy/Enemy.h"




UBTTask_PlayerAttack::UBTTask_PlayerAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "PlayerAttack";
}

EBTNodeResult::Type UBTTask_PlayerAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UE_LOG(LogTemp, Warning, TEXT("AIdamage"));
    // AIController Ȯ��
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("AIdamage2"));
        return EBTNodeResult::Failed;
    }

    // �����忡�� �÷��̾� ���� ��������
    AProjectKLBCharacter* TargetPlayer = Cast<AProjectKLBCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject("TargetPlayer"));
    if (!TargetPlayer)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetPlayer not found in Blackboard!"));
        return EBTNodeResult::Failed;
    }

    // ���ʹ� ĳ���� ��������
    AEnemy* EnemyCharacter = Cast<AEnemy>(AIController->GetPawn());
    if (!EnemyCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("AI Pawn is not an AEnemyCharacter!"));
        return EBTNodeResult::Failed;
    }


    //// ���� �ִϸ��̼� ����
    if (TargetPlayer)
    {
        EnemyCharacter->PlayAttackAnimation(); // ������: 10
    }

    return EBTNodeResult::Succeeded;
}


