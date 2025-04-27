// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"



class AGridManager;

UCLASS()
class PROJECTKLB_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // ���ʹ� Ŭ���� ����
    UPROPERTY(EditAnywhere, Category = "Enemy")
    TSubclassOf<AActor> EnemyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    TSubclassOf<class AGridManager> GridManagerClass;

    // ���ʹ̸� ��ġ�� ��ǥ
    UPROPERTY(EditAnywhere, Category = "Enemy")
    TArray<FIntPoint> EnemySpawnCoordinates;

    // ���ʹ̸� ��ġ�ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void SpawnEnemies();

    // Ÿ�� �Ŵ��� ����
    UPROPERTY(EditAnywhere, Category = "Grid")
    AGridManager* GridManager;

    // ���ʹ̰� ����� �� ȣ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void OnEnemyDestroyed();

    // ��� ���ʹ̰� ������� �� UI�� ���� �Լ� (��: Delegate ȣ�� ��)
    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void ShowAllEnemiesDefeatedUI();

    // ���� ����ִ� ���ʹ� ��
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
    int32 AliveEnemyCount;

};
