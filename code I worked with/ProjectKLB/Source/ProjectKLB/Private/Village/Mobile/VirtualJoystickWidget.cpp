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

        // ���� ����ȭ
        FVector2D NormalizedVector = Delta / MaxDistance;
        OnMovementUpdated.Broadcast(NormalizedVector);

        // ���̽�ƽ �� �̹����� ��ġ ������Ʈ (Render Transform �̿�)
        if (JoystickThumbImage)
        {
            // RenderTransform���� Translation ���� �����մϴ�.
            FWidgetTransform Transform = JoystickThumbImage->RenderTransform;
            Transform.Translation = Delta;  // ���̽� �߽��� �������� �̵�
            JoystickThumbImage->SetRenderTransform(Transform);
        }
    }

}

void UVirtualJoystickWidget::HandleTouchStarted(FVector2D TouchPosition)
{
    bIsTouching = true;
    // ��ġ ���� �� ���� ��ġ ��ġ�� ���̽�ƽ�� �߽����� ����
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

    // ��ġ ���� �� �� �̹����� ����ġ�� ���ͽ�Ű��
    if (JoystickThumbImage)
    {
        FWidgetTransform Transform = JoystickThumbImage->RenderTransform;
        Transform.Translation = FVector2D::ZeroVector;
        JoystickThumbImage->SetRenderTransform(Transform);
        UE_LOG(LogTemp, Warning, TEXT("HandleTouchEnded"));
    }
}


