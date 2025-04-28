// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Crash/Skill2DCollisionActor.h"
#include "Skill2DConeCollisionActor.generated.h"

/**
 * 원뿔 모양 충돌 판정을 위한 액터.
 * 이 클래스는 원뿔의 apex(액터의 위치)를 기준으로, 지정한 방향(ConeDirection)으로 ConeRange 길이 내에서,
 * ConeAngle (전체 각도) 이내에 있는 대상과의 충돌 판정을 수행
 */
UCLASS()
class PROJECT_S_API ASkill2DConeCollisionActor : public ASkill2DCollisionActor
{
	GENERATED_BODY()
	
public:
    ASkill2DConeCollisionActor();

    // 원뿔 충돌 범위
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision|Cone")
    float ConeRange;

    // 원뿔 전체 각도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision|Cone")
    float ConeAngle;

    // 원뿔의 방향
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision|Cone")
    FVector ConeDirection;

    bool Bso = true;

protected:
    // 부모 순수 가상함수를 오버라이드하여 원뿔 충돌 형태 초기화
    virtual void InitializeCollision(const FSkillData& InSkillData) override;

    // 디버그 시각화
    virtual void DrawCollisionDebug() override;

public:
    virtual void Tick(float DeltaTime) override;

};
