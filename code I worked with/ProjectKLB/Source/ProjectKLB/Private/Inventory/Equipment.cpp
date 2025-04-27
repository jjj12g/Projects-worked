// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Equipment.h"
#include "ProjectKLBCharacter.h"
#include "Components/Image.h"
#include "Inventory/EquipmentSlot.h"
#include "Player/CharacterStatComponent.h"
#include "Components/TextBlock.h"



void UEquipment::InitializeEquipment(AProjectKLBCharacter* InCharacter)
{
    if (!InCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("InitializeEquipment: InCharacter is null"));
        return;
    }

    // 각 슬롯에 해당하는 장비 데이터를 전달합니다.
    // 예시: AProjectKLBCharacter 클래스에 각 슬롯에 대한 변수(Weapon, Helmet, Chest, Legs)가 있다고 가정합니다.
    if (WeaponSlot)
    {
        WeaponSlot->InitializeEquipmentSlot(InCharacter->StatComponent->Weapon,0);
    }
    if (HelmetSlot)
    {
        HelmetSlot->InitializeEquipmentSlot(InCharacter->StatComponent->Helmet,1);
    }
    if (ChestSlot)
    {
        ChestSlot->InitializeEquipmentSlot(InCharacter->StatComponent->Chest,2);
    }
    if (LegSlot)
    {
        LegSlot->InitializeEquipmentSlot(InCharacter->StatComponent->Legs,3);
    }


   /* if (InCharacter->StatComponent)
    {
        CharNameText->SetText(FText::FromName(InCharacter->StatComponent->BaseStats.Name));
    }*/


    CharHealthText->SetText(FText::AsNumber(InCharacter->CurrentHP));
    CharAttackText->SetText(FText::AsNumber(InCharacter->CurrentAttack));
    CharDefenseText->SetText(FText::AsNumber(InCharacter->CurrentAttack));
    CharSpeedText->SetText(FText::AsNumber(InCharacter->CurrentSpeed));



    UE_LOG(LogTemp, Log, TEXT("Equipment UI updated for character: %s"), *InCharacter->GetName());
}
