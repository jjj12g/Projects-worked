// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "CharacterStat/CharacterStat.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"



class UBehaviorTree;
class UAnimMontage;
class AProjectKLBCharacter;
class UWidgetComponent;
class ATile;
class AGridManager;
class AEnemySpawner;


UCLASS()
class PROJECTKLB_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 데미지 받는 함수
	void Damage(float DamageAmount);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = 100.0f;

	UBehaviorTree* GetBehaviorTree() const;
	
	// 타겟 플레이어 설정 및 반환
	void SetTargetPlayer(AProjectKLBCharacter* Player);
	AProjectKLBCharacter* GetTargetPlayer() const;

	// 스탯 초기화 함수
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void InitializeStats();

	// 에너미의 현재 스탯
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	FCharacterStat CharacterStats;

	// 에너미의 이름
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	FName EnemyName;

	// AttackMontage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AttackMontage;


	// 다른 에너미 있는지 확인
	void MoveToTile(ATile* TargetTile);

	// 에너미 있을 시 대체타일
	ATile* FindAlternativeTile(ATile* OccupiedTile);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	AGridManager* GridManager;


	// 현재 위치한 타일
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	ATile* CurrentTile = nullptr;

	 
	// 플레이어 HP 바 Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	float CurrentHP;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	float MaxHP;

	
	// Enemy 이동 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 300.0f; 

	// 공격 애니메이션
	void PlayAttackAnimation();

	FTimerHandle AttackTimerHandle;

	// 플레이어를 바라보는 함수
	void LookAtPlayer();

	// 스포너 참조를 위한 함수 및 변수
	UPROPERTY()
	class AEnemySpawner* EnemySpawner;

	void SetEnemySpawner(AEnemySpawner* Spawner) { EnemySpawner = Spawner; }

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess="true"))  //AllowPrivateAccess?
	UBehaviorTree* Tree;

public:
	

private:
	AProjectKLBCharacter* TargetPlayer;

	

};
