// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CharacterIcon/CharacterIconWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "ProjectKLBCharacter.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void UCharacterIconWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CharacterClass)
    {
        // ĳ���� Ŭ������ �⺻ ��ü�� �����ɴϴ�.
        AProjectKLBCharacter* DefaultChar = CharacterClass->GetDefaultObject<AProjectKLBCharacter>();
        if (DefaultChar && DefaultChar->CharacterIcon && IconImage)
        {
            // UTexture2D*�� FSlateBrush�� �����Ͽ� UImage�� �ݿ�
            FSlateBrush Brush;
            Brush.SetResourceObject(DefaultChar->CharacterIcon);
            IconImage->SetBrush(Brush);
        }
    }

}

void UCharacterIconWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    UDragDropOperation* DragOp = NewObject<UDragDropOperation>(this, UDragDropOperation::StaticClass());
    // �巡�� �ð������� ������ �巡�� ���־�
    DragOp->DefaultDragVisual = this;

    // Payload�� ĳ���� Ŭ������ ��´�
    DragOp->Payload = CharacterClass;

    OutOperation = DragOp;
}

FReply UCharacterIconWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
    }
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
