// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CharacterStat/CharacterStatsTable.h"
#include "CharacterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_S_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()


public:
    UCharacterStatComponent();

    // 데이터 테이블
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    UDataTable* CharacterStatsDataTable;
    
    // 사용 할 이름
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    FName StatRowName;

    // 현재 캐릭터의 스탯 값 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    FCharacterStats CurrentStats;

    // 데이터 테이블 스탯 로드 함수
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void LoadStatsFromDataTable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


};
