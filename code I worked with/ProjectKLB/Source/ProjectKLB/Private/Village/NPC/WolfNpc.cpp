// Fill out your copyright notice in the Description page of Project Settings.


#include "Village/NPC/WolfNpc.h"
#include "Village/VillageCharacter.h"
#include "Player/CharacterStatComponent.h"



AWolfNpc::AWolfNpc()
{
    // 이 NPC는 "Wolf"라는 StatRowName을 사용하여 데이터 테이블의 "Wolf" 행을 참조하도록 합니다.
    StatRowName = FName("Wolf");

}

FText AWolfNpc::GetDialogueForPlayer(AVillageCharacter* PlayerCharacter) const
{
    // 이제 PlayerCharacter가 아니라, NPC 자신의 StatComponent를 통해 호감도를 가져옵니다.
    if (!StatComponent)
    {
        return FText::FromString("..."); // 기본 대사
    }

    int32 NPCAffinity = StatComponent->FinalStats.Favorability;
    
    // 예시: NPC 자신의 호감도가 높으면 다른 대사, 낮으면 다른 대사를 출력
    if (NPCAffinity >= 90)
    {
        return FText::FromString("I trust you, friend. Let's work together. 90");
    }
    else if (NPCAffinity >= 60)
    {
        return FText::FromString("You seem alright, but don't get too close. 60");
    }
    else if (NPCAffinity >= 30)
    {
        return FText::FromString("30");
    }
    else
    {
        return FText::FromString("Stay away from me! 0");
    }
}

void AWolfNpc::InitializeStats()
{
    // 기본 스탯 초기화(부모 클래스에서 설정한 로직)
    Super::InitializeStats();

    // 필요하다면 Wolf NPC만의 추가 초기화 로직을 여기에 추가합니다.
    // 예를 들어, Wolf는 공격력이 약간 높다고 가정하면:
   //CharacterStats.Attack += 5;

    UE_LOG(LogTemp, Log, TEXT("AWolfNPC: Additional initialization complete. Attack = %d"), CharacterStats.Attack);

}
