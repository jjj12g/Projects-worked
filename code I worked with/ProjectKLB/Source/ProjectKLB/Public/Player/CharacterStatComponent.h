// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStat/CharacterStat.h"
#include "Inventory/ItemData.h" // 아이템 데이터 구조체 헤더 포함
#include "CharacterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTKLB_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


    // 기본 스탯 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    FCharacterStat BaseStats;

    // 최종 스탯 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    FCharacterStat FinalStats;

    // 장비 슬롯에 장착된 아이템
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FItemData Weapon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FItemData Helmet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FItemData Chest;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FItemData Legs;


    // 데이터 테이블에서 불러온 기본 스탯으로 초기화 
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void InitializeBaseStats(const FCharacterStat& InStats);

    // 장비 보너스를 적용하여 최종 스탯을 재계산 
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void RecalculateStats();

    // 호감도 상승
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void IncreaseFavorability(int32 Amount);
		
};
