// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMoveTask/BTTask_EndTurn.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectKLBPlayerController.h"


UBTTask_EndTurn::UBTTask_EndTurn(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "EndTurn";
}

EBTNodeResult::Type UBTTask_EndTurn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AProjectKLBPlayerController* PC = Cast<AProjectKLBPlayerController>(GetWorld()->GetFirstPlayerController());

	Blackboard = AIController->GetBlackboardComponent();
	if (Blackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("EndTurn"));
		PC->PlayerTurn();
		Blackboard->SetValueAsBool("PlayerCardUsed?", false);
		return EBTNodeResult::Succeeded;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("NotEndTurn"));
	return EBTNodeResult::Failed;
}
