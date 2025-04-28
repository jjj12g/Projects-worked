// Fill out your copyright notice in the Description page of Project Settings.


#include "Village/Mobile/VirtualJoystickWidget.h"
#include "Components/Image.h"



void UVirtualJoystickWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (bIsTouching)
    {
        FVector2D Delta = CurrentTouchLocation - JoystickCenter;

        double DeltaSize = Delta.Size();
        if (DeltaSize > MaxDistance)
        {
            Delta = Delta / DeltaSize * MaxDistance;
        }

        // 백터 정규화
        FVector2D NormalizedVector = Delta / MaxDistance;
        OnMovementUpdated.Broadcast(NormalizedVector);

        // 조이스틱 썸 이미지의 위치 업데이트 (Render Transform 이용)
        if (JoystickThumbImage)
        {
            // RenderTransform에서 Translation 값을 조정합니다.
            FWidgetTransform Transform = JoystickThumbImage->RenderTransform;
            Transform.Translation = Delta;  // 베이스 중심을 기준으로 이동
            JoystickThumbImage->SetRenderTransform(Transform);
        }
    }

}

void UVirtualJoystickWidget::HandleTouchStarted(FVector2D TouchPosition)
{
    bIsTouching = true;
    // 터치 시작 시 현재 터치 위치를 조이스틱의 중심으로 설정
    JoystickCenter = TouchPosition;
    CurrentTouchLocation = TouchPosition;
    UE_LOG(LogTemp, Warning, TEXT("HandleTouchStarted"));
}

void UVirtualJoystickWidget::HandleTouchMoved(FVector2D TouchPosition)
{
    if (bIsTouching)
    {
        CurrentTouchLocation = TouchPosition;
        UE_LOG(LogTemp, Warning, TEXT("HandleTouchMoved"));
    }
}

void UVirtualJoystickWidget::HandleTouchEnded()
{
    bIsTouching = false;
    OnMovementUpdated.Broadcast(FVector2D::ZeroVector);

    // 터치 종료 시 썸 이미지를 원위치로 복귀시키기
    if (JoystickThumbImage)
    {
        FWidgetTransform Transform = JoystickThumbImage->RenderTransform;
        Transform.Translation = FVector2D::ZeroVector;
        JoystickThumbImage->SetRenderTransform(Transform);
        UE_LOG(LogTemp, Warning, TEXT("HandleTouchEnded"));
    }
}


