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

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    bool bss = true;

    //FVector BoxExtent;

protected:
    // �ڽ� �浹 ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    UBoxComponent* CollisionBox;

    // ����� �ð�ȭ (�ڽ�)
    virtual void DrawCollisionDebug() override;

};
