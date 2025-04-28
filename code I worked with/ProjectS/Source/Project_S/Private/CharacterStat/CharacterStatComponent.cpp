// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatComponent.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


}



// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

    // 게임 시작 시 데이터 테이블에서 스탯 로드
    LoadStatsFromDataTable();
}




void UCharacterStatComponent::LoadStatsFromDataTable()
{
    if (CharacterStatsDataTable)
    {
        static const FString ContextString(TEXT("CharacterStatsContext"));
        FCharacterStats* Stats = CharacterStatsDataTable->FindRow<FCharacterStats>(StatRowName, ContextString, true);
        if (Stats)
        {
            CurrentStats = *Stats; // 데이터를 복사
            UE_LOG(LogTemp, Log, TEXT("Loaded stats for %s: Health=%d, Mana=%d, Attack=%d, Defense=%d"),
                *StatRowName.ToString(), CurrentStats.Health, CurrentStats.Mana, CurrentStats.Attack, CurrentStats.Defense);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No stats found for row %s in the data table."), *StatRowName.ToString());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CharacterStatsDataTable is not assigned in %s."), *GetName());
    }

}
