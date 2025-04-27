// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
 class UBlackboardComponent;
 class UBehaviorTreeComponent;
 class ATile;
 class UBlackboardData;
 class AProjectKLBCharacter;


UCLASS()
class PROJECTKLB_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()


public:
	//UBlackboardComponent* BlackboardComponent;


	

	// **�����̺�� Ʈ���� ������**
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBlackboardData* Blackboard;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent;

	TArray<ATile*> CurrentPath;

	// ���� ����� �÷��̾ ã�� static �Լ�
	static AProjectKLBCharacter* GetClosestPlayer(UWorld* World, AActor* EnemyActor);

	AProjectKLBCharacter* TargetPlayer;

	// ��� ���� �Լ�
	void SetPath(const TArray<ATile*>& NewPath);

	// ���� Ÿ�� ��������
	ATile* GetNextTile();

	// ��� ����
	void AdvancePath();

	void AttackPlayer();

	explicit AEnemyAIController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	

};
