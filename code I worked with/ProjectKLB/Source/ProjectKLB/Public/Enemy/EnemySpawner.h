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

    // 에너미 클래스 설정
    UPROPERTY(EditAnywhere, Category = "Enemy")
    TSubclassOf<AActor> EnemyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    TSubclassOf<class AGridManager> GridManagerClass;

    // 에너미를 배치할 좌표
    UPROPERTY(EditAnywhere, Category = "Enemy")
    TArray<FIntPoint> EnemySpawnCoordinates;

    // 에너미를 배치하는 함수
    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void SpawnEnemies();

    // 타일 매니저 참조
    UPROPERTY(EditAnywhere, Category = "Grid")
    AGridManager* GridManager;

    // 에너미가 사망할 때 호출할 함수
    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void OnEnemyDestroyed();

    // 모든 에너미가 사망했을 때 UI를 띄우는 함수 (예: Delegate 호출 등)
    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void ShowAllEnemiesDefeatedUI();

    // 현재 살아있는 에너미 수
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
    int32 AliveEnemyCount;

};
