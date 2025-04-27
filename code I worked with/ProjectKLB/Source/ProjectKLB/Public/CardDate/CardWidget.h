// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardDate/CardData.h"  // FCardData ����ü ����
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
    
    // ī�� ������
    UPROPERTY(BlueprintReadWrite, Category = "Card")
    FCardData CardData;

    // ī�� ��� �̺�Ʈ
    UFUNCTION(BlueprintCallable, Category = "Card")
    void OnCardUsed();

    // ī�� �̸��� ������ UI�� ������ �Լ���
    void InitializeCard(const FCardData& CardData);

    // ī�� �����͸� ��ȯ�ϴ� �Լ�
    const FCardData& GetCardData() const { return CardData;}

    // �巡�� ���� ����
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    // �巡�� �� �߻��ϴ� ����
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

    // �巡�װ� ������ �� ó��
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
    // UI ��ҵ�
    UPROPERTY(meta = (BindWidget))
    UTextBlock* CardNameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CardDescriptionText;

    UPROPERTY(meta = (BindWidget))
    UImage* CardIconImage;

    // �巡�� �� ��ӿ��� Ȱ���� GridPanel
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Layout")
    UGridPanel* CardPanel;






};
