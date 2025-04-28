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

    // ������ ���̺�
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    UDataTable* CharacterStatsDataTable;
    
    // ��� �� �̸�
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    FName StatRowName;

    // ���� ĳ������ ���� �� 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    FCharacterStats CurrentStats;

    // ������ ���̺� ���� �ε� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void LoadStatsFromDataTable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


};
