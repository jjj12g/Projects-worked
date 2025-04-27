// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveWidget.generated.h"

/**
 * 
 */

 class UButton;

UCLASS()
class PROJECTKLB_API USaveWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
    // ���� �����Ϳ��� ��ư �̸��� ��ġ�ϵ��� BindWidget �մϴ�.
    UPROPERTY(meta = (BindWidget))
    UButton* SlotButton1;

    UPROPERTY(meta = (BindWidget))
    UButton* SlotButton2;

    UPROPERTY(meta = (BindWidget))
    UButton* SlotButton3;

    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnSlotButton1Clicked();

    UFUNCTION()
    void OnSlotButton2Clicked();

    UFUNCTION()
    void OnSlotButton3Clicked();

    // ���õ� ���� ��ȣ�� ���� ���� �ε� ó��
    void LoadGameSlot(int32 SlotIndex);

	
};
