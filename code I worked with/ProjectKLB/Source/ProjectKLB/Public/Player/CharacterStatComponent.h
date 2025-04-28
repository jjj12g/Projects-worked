// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStat/CharacterStat.h"
#include "Inventory/ItemData.h" // ������ ������ ����ü ��� ����
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


    // �⺻ ���� 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    FCharacterStat BaseStats;

    // ���� ���� 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    FCharacterStat FinalStats;

    // ��� ���Կ� ������ ������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FItemData Weapon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FItemData Helmet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FItemData Chest;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    FItemData Legs;


    // ������ ���̺��� �ҷ��� �⺻ �������� �ʱ�ȭ 
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void InitializeBaseStats(const FCharacterStat& InStats);

    // ��� ���ʽ��� �����Ͽ� ���� ������ ���� 
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void RecalculateStats();

    // ȣ���� ���
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void IncreaseFavorability(int32 Amount);
		
};
