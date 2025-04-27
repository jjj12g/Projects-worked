// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Crash/Skill2DCollisionActor.h"
#include "Skill2DSphereCollisionActor.generated.h"

/**
 * 
 */

 class USphereComponent;
 class ABaseCharacters;

UCLASS()
class PROJECT_S_API ASkill2DSphereCollisionActor : public ASkill2DCollisionActor
{
	GENERATED_BODY()
	
public:
    ASkill2DSphereCollisionActor();

    // 가상 함수 오버라이드: 데이터 테이블 값에 따라 충돌 컴포넌트 초기화
    virtual void InitializeCollision(const FSkillData& InSkillData) override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    bool bs = true;

protected:
    // 충돌을 위한 스피어 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    USphereComponent* CollisionSphere;

    // 디버그 시각화 (스피어)
    virtual void DrawCollisionDebug() override;

    // 이미 충돌이 처리된 플레이어들을 저장 (중복 처리 방지)
    TSet<ABaseCharacters*> ProcessedOverlaps;

};
