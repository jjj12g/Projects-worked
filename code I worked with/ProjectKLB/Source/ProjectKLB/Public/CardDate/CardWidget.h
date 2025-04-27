// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardDate/CardData.h"  // FCardData 구조체 포함
#include "CardWidget.generated.h"


/**
 * 
 */

class UTextBlock;
class UImage;
class UDragDropOperation;
class UGridPanel;
class AProjectKLBPlayerController;

UCLASS()
class PROJECTKLB_API UCardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    
    // 카드 데이터
    UPROPERTY(BlueprintReadWrite, Category = "Card")
    FCardData CardData;

    // 카드 사용 이벤트
    UFUNCTION(BlueprintCallable, Category = "Card")
    void OnCardUsed();

    // 카드 이름과 설명을 UI에 적용할 함수들
    void InitializeCard(const FCardData& CardData);

    // 카드 데이터를 반환하는 함수
    const FCardData& GetCardData() const { return CardData;}

    // 드래그 동작 시작
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    // 드래그 중 발생하는 동작
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

    // 드래그가 끝났을 때 처리
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
    // UI 요소들
    UPROPERTY(meta = (BindWidget))
    UTextBlock* CardNameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CardDescriptionText;

    UPROPERTY(meta = (BindWidget))
    UImage* CardIconImage;

    // 드래그 앤 드롭에서 활용할 GridPanel
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Layout")
    UGridPanel* CardPanel;






};
