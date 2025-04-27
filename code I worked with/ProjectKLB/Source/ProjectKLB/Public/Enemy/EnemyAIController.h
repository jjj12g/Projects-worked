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


	

	// **비헤이비어 트리와 블랙보드**
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBlackboardData* Blackboard;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent;

	TArray<ATile*> CurrentPath;

	// 가장 가까운 플레이어를 찾는 static 함수
	static AProjectKLBCharacter* GetClosestPlayer(UWorld* World, AActor* EnemyActor);

	AProjectKLBCharacter* TargetPlayer;

	// 경로 설정 함수
	void SetPath(const TArray<ATile*>& NewPath);

	// 다음 타일 가져오기
	ATile* GetNextTile();

	// 경로 진행
	void AdvancePath();

	void AttackPlayer();

	explicit AEnemyAIController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	

};
