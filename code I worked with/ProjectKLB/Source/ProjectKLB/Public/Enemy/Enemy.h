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

	// ������ �޴� �Լ�
	void Damage(float DamageAmount);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = 100.0f;

	UBehaviorTree* GetBehaviorTree() const;
	
	// Ÿ�� �÷��̾� ���� �� ��ȯ
	void SetTargetPlayer(AProjectKLBCharacter* Player);
	AProjectKLBCharacter* GetTargetPlayer() const;

	// ���� �ʱ�ȭ �Լ�
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void InitializeStats();

	// ���ʹ��� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	FCharacterStat CharacterStats;

	// ���ʹ��� �̸�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	FName EnemyName;

	// AttackMontage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AttackMontage;


	// �ٸ� ���ʹ� �ִ��� Ȯ��
	void MoveToTile(ATile* TargetTile);

	// ���ʹ� ���� �� ��üŸ��
	ATile* FindAlternativeTile(ATile* OccupiedTile);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	AGridManager* GridManager;


	// ���� ��ġ�� Ÿ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	ATile* CurrentTile = nullptr;

	 
	// �÷��̾� HP �� Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	float CurrentHP;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	float MaxHP;

	
	// Enemy �̵� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 300.0f; 

	// ���� �ִϸ��̼�
	void PlayAttackAnimation();

	FTimerHandle AttackTimerHandle;

	// �÷��̾ �ٶ󺸴� �Լ�
	void LookAtPlayer();

	// ������ ������ ���� �Լ� �� ����
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
