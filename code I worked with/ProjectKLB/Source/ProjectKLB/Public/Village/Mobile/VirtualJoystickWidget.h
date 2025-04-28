// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VirtualJoystickWidget.generated.h"

/**
 * 
 */

class UImage;

 // delegate ����: �Ķ���ͷ� FVector2D�� ����
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMovementUpdated, FVector2D, MovementVector);

UCLASS()
class PROJECTKLB_API UVirtualJoystickWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    // ��ġ �߿� ������Ʈ�� �̵� ���� 
    UPROPERTY(BlueprintAssignable, Category = "Joystick")
    FOnMovementUpdated OnMovementUpdated;


    // �ִ� �̵� ������ 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick")
    float MaxDistance = 100.0f;


    // ��ġ �̺�Ʈ ó�� 
    UFUNCTION(BlueprintCallable, Category = "Joystick")
    void HandleTouchStarted(FVector2D TouchPosition);

    UFUNCTION(BlueprintCallable, Category = "Joystick")
    void HandleTouchMoved(FVector2D TouchPosition);

    UFUNCTION(BlueprintCallable, Category = "Joystick")
    void HandleTouchEnded();


protected:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    

    // ���� ��ġ�� �������̸� true
    bool bIsTouching = false;


    // ��ġ ���� ���� �߽� ��ġ 
    FVector2D JoystickCenter;

    // ���� ��ġ ��ġ
    FVector2D CurrentTouchLocation;


    // �Ʒ� ���� �������� UMG �����Ϳ��� ���ε�
    UPROPERTY(meta = (BindWidget))
    UImage* JoystickBaseImage;

    UPROPERTY(meta = (BindWidget))
    UImage* JoystickThumbImage;
   


};
