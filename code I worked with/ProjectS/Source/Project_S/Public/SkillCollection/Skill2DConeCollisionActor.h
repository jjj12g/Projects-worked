// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Crash/Skill2DCollisionActor.h"
#include "Skill2DConeCollisionActor.generated.h"

/**
 * 원뿔 모양 충돌 판정을 위한 액터.
 * 이 클래스는 원뿔의 apex(액터의 위치)를 기준으로, 지정한 방향(ConeDirection)으로 ConeRange 길이 내에서,
 * ConeAngle (전체 각도) 이내에 있는 대상과의 충돌 판정을 수행합니다.
 */
UCLASS()
class PROJECT_S_API ASkill2DConeCollisionActor : public ASkill2DCollisionActor
{
	GENERATED_BODY()
	
public:
    ASkill2DConeCollisionActor();

    // 원뿔 충돌 범위 (최대 길이, 센티미터 단위)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision|Cone")
    float ConeRange;

    // 원뿔 전체 각도 (도 단위, 예: 60도이면 반각 30도)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision|Cone")
    float ConeAngle;

    // 원뿔의 방향 (일반적으로 액터의 Forward Vector)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision|Cone")
    FVector ConeDirection;

    bool Bso = true;

protected:
    // 부모 순수 가상함수를 오버라이드하여 원뿔 충돌 형태 초기화
    virtual void InitializeCollision(const FSkillData& InSkillData) override;

    // 디버그 시각화를 위한 오버라이드 함수: 원뿔을 DrawDebugCone()으로 그립니다.
    virtual void DrawCollisionDebug() override;

public:
    // 매 프레임 대상(예, 플레이어)과의 원뿔 충돌을 커스텀 판정하고 디버그 출력하는 함수
    virtual void Tick(float DeltaTime) override;

};
