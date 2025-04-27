// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Equipment.generated.h"

/**
 * 
 */

 class AProjectKLBCharacter;
 class UEquipmentSlot;
 class UTextBlock;


UCLASS()
class PROJECTKLB_API UEquipment : public UUserWidget
{
	GENERATED_BODY()
	

public:
    // 장비 정보를 초기화하는 함수
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void InitializeEquipment(AProjectKLBCharacter* InCharacter);

protected:
    // 각 장비 슬롯 UI 요소 (예: 이미지, 텍스트 등)
    // 각 슬롯 위젯을 UMG에서 BindWidget으로 연결
    UPROPERTY(meta = (BindWidget))
    UEquipmentSlot* WeaponSlot;

    UPROPERTY(meta = (BindWidget))
    UEquipmentSlot* HelmetSlot;

    UPROPERTY(meta = (BindWidget))
    UEquipmentSlot* ChestSlot;

    UPROPERTY(meta = (BindWidget))
    UEquipmentSlot* LegSlot;

    // 스탯 표기
    //UPROPERTY(meta = (BindWidget))
    //UTextBlock* CharNameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CharHealthText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CharAttackText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CharDefenseText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CharSpeedText;

};
