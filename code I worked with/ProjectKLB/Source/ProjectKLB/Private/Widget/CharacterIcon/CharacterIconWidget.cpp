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
        // 캐릭터 클래스의 기본 객체를 가져옵니다.
        AProjectKLBCharacter* DefaultChar = CharacterClass->GetDefaultObject<AProjectKLBCharacter>();
        if (DefaultChar && DefaultChar->CharacterIcon && IconImage)
        {
            // UTexture2D*를 FSlateBrush로 설정하여 UImage에 반영
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
    // 드래그 시각적으로 보여줄 드래그 비주얼
    DragOp->DefaultDragVisual = this;

    // Payload에 캐릭터 클래스를 담는다
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
