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
    // ��� ������ �ʱ�ȭ�ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void InitializeEquipment(AProjectKLBCharacter* InCharacter);

protected:
    // �� ��� ���� UI ��� (��: �̹���, �ؽ�Ʈ ��)
    // �� ���� ������ UMG���� BindWidget���� ����
    UPROPERTY(meta = (BindWidget))
    UEquipmentSlot* WeaponSlot;

    UPROPERTY(meta = (BindWidget))
    UEquipmentSlot* HelmetSlot;

    UPROPERTY(meta = (BindWidget))
    UEquipmentSlot* ChestSlot;

    UPROPERTY(meta = (BindWidget))
    UEquipmentSlot* LegSlot;

    // ���� ǥ��
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
