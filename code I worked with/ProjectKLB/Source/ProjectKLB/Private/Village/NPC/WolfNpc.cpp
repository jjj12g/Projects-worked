// Fill out your copyright notice in the Description page of Project Settings.


#include "Village/NPC/WolfNpc.h"
#include "Village/VillageCharacter.h"
#include "Player/CharacterStatComponent.h"



AWolfNpc::AWolfNpc()
{
    // �� NPC�� "Wolf"��� StatRowName�� ����Ͽ� ������ ���̺��� "Wolf" ���� �����ϵ��� �մϴ�.
    StatRowName = FName("Wolf");

}

FText AWolfNpc::GetDialogueForPlayer(AVillageCharacter* PlayerCharacter) const
{
    // ���� PlayerCharacter�� �ƴ϶�, NPC �ڽ��� StatComponent�� ���� ȣ������ �����ɴϴ�.
    if (!StatComponent)
    {
        return FText::FromString("..."); // �⺻ ���
    }

    int32 NPCAffinity = StatComponent->FinalStats.Favorability;
    
    // ����: NPC �ڽ��� ȣ������ ������ �ٸ� ���, ������ �ٸ� ��縦 ���
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
    // �⺻ ���� �ʱ�ȭ(�θ� Ŭ�������� ������ ����)
    Super::InitializeStats();

    // �ʿ��ϴٸ� Wolf NPC���� �߰� �ʱ�ȭ ������ ���⿡ �߰��մϴ�.
    // ���� ���, Wolf�� ���ݷ��� �ణ ���ٰ� �����ϸ�:
   //CharacterStats.Attack += 5;

    UE_LOG(LogTemp, Log, TEXT("AWolfNPC: Additional initialization complete. Attack = %d"), CharacterStats.Attack);

}
