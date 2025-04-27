// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardListWidget.generated.h"

/**
 * 
 */
 class UHorizontalBox;
 class UDropZoneWidget;
 class UCardWidget;


UCLASS()
class PROJECTKLB_API UCardListWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 카드 추가 함수
    UFUNCTION(BlueprintCallable, Category = "Card")
    void AddCard(UCardWidget* CardWidget);

    // UI 갱신 함수
    void UpdateCardUI();

    void ClearAllCards();

      
    void SetCardVisibility(ESlateVisibility NewVisibility);
    
private:
    // 카드를 담는 컨테이너
    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* CardContainer;

    // 드롭 영역
    //UPROPERTY(meta = (BindWidget))
    //UDropZoneWidget* DropZone;

    // 카드 위젯 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UCardWidget> CardWidgetClass;

    // 최대 표시 카드 수
    UPROPERTY(EditDefaultsOnly, Category = "Card UI")
    int32 MaxDisplayedCards = 4;
};
