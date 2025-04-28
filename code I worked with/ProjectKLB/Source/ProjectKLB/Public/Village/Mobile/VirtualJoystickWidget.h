// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VirtualJoystickWidget.generated.h"

/**
 * 
 */

class UImage;

 // delegate 선언: 파라미터로 FVector2D를 전달
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMovementUpdated, FVector2D, MovementVector);

UCLASS()
class PROJECTKLB_API UVirtualJoystickWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    // 터치 중에 업데이트된 이동 벡터 
    UPROPERTY(BlueprintAssignable, Category = "Joystick")
    FOnMovementUpdated OnMovementUpdated;


    // 최대 이동 반지름 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joystick")
    float MaxDistance = 100.0f;


    // 터치 이벤트 처리 
    UFUNCTION(BlueprintCallable, Category = "Joystick")
    void HandleTouchStarted(FVector2D TouchPosition);

    UFUNCTION(BlueprintCallable, Category = "Joystick")
    void HandleTouchMoved(FVector2D TouchPosition);

    UFUNCTION(BlueprintCallable, Category = "Joystick")
    void HandleTouchEnded();


protected:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    

    // 현재 터치가 진행중이면 true
    bool bIsTouching = false;


    // 터치 시작 시의 중심 위치 
    FVector2D JoystickCenter;

    // 현재 터치 위치
    FVector2D CurrentTouchLocation;


    // 아래 위젯 변수들은 UMG 에디터에서 바인딩
    UPROPERTY(meta = (BindWidget))
    UImage* JoystickBaseImage;

    UPROPERTY(meta = (BindWidget))
    UImage* JoystickThumbImage;
   


};
