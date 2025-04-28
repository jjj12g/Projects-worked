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

    // ������ ���̺� ���� ���� �ڽ� ������Ʈ�� �ʱ�ȭ�ϴ� �Լ�
    virtual void InitializeCollision(const FSkillData& InSkillData) override;

    virtual void Tick(float DeltaTime) override;

    bool bDebugBox = true;


protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    UBoxComponent* CollisionBox;

    // ����� �ð�ȭ
    virtual void DrawCollisionDebug() override;

};
