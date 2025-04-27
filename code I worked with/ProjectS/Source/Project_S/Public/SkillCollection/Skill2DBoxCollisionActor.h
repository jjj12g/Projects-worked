// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Crash/Skill2DCollisionActor.h"
#include "Skill2DBoxCollisionActor.generated.h"

/**
 * 
 */

 class UBoxComponent;

UCLASS()
class PROJECT_S_API ASkill2DBoxCollisionActor : public ASkill2DCollisionActor
{
	GENERATED_BODY()
	
public:
    ASkill2DBoxCollisionActor();

    // 데이터 테이블 값에 따라 박스 컴포넌트를 초기화하는 함수
    virtual void InitializeCollision(const FSkillData& InSkillData) override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    bool bss = true;

    //FVector BoxExtent;

protected:
    // 박스 충돌 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    UBoxComponent* CollisionBox;

    // 디버그 시각화 (박스)
    virtual void DrawCollisionDebug() override;

};
