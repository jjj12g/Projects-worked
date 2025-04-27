// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DialogueData/WolfDialogueData.h" // 호감도 구조체를 포함하는 헤더
#include "CharacterStat/CharacterStat.h"
#include "FavorabilityBaseNpcCharacter.generated.h"

UCLASS()
class PROJECTKLB_API AFavorabilityBaseNpcCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFavorabilityBaseNpcCharacter();


    // NPC 스탯 초기화 함수
    UFUNCTION(BlueprintCallable, Category = "Stats")
    virtual void InitializeStats();

    // 캐릭터 스탯 (데이터 테이블에서 가져온 값)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    struct FCharacterStat CharacterStats;

    // NPC가 사용할 StatComponent (스탯 데이터를 관리)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    class UCharacterStatComponent* StatComponent;

    // NPC 이름(데이터 테이블 행의 키로 사용)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FName StatRowName;

    // 플레이어의 호감도에 따라 대사를 반환하는 가상 함수
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    virtual FText GetDialogueForPlayer(class AVillageCharacter* PlayerCharacter) const;


    // 플레이어와 상호작용할 때 호출되는 가상 함수
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    virtual void Interact(class AVillageCharacter* PlayerCharacter);

protected:
    virtual void BeginPlay() override;

    // NPC 이름 및 기본 대사 정보 (추후 애니메이션, AI 등과 연동 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FText NPCName;

    // 대화 데이터 테이블 (호감도에 따른 대사 데이터를 참조할 수 있음)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    class UDataTable* DialogueDataTable;

};
