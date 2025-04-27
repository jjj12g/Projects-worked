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
    // 위젯 에디터에서 버튼 이름과 일치하도록 BindWidget 합니다.
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

    // 선택된 슬롯 번호에 따라 게임 로드 처리
    void LoadGameSlot(int32 SlotIndex);

	
};
