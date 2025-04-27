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
    // ī�� �߰� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Card")
    void AddCard(UCardWidget* CardWidget);

    // UI ���� �Լ�
    void UpdateCardUI();

    void ClearAllCards();

      
    void SetCardVisibility(ESlateVisibility NewVisibility);
    
private:
    // ī�带 ��� �����̳�
    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* CardContainer;

    // ��� ����
    //UPROPERTY(meta = (BindWidget))
    //UDropZoneWidget* DropZone;

    // ī�� ���� Ŭ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UCardWidget> CardWidgetClass;

    // �ִ� ǥ�� ī�� ��
    UPROPERTY(EditDefaultsOnly, Category = "Card UI")
    int32 MaxDisplayedCards = 4;
};
