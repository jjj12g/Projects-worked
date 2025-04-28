// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Crash/Skill2DCollisionActor.h"
#include "Skill2DConeCollisionActor.generated.h"

/**
 * ���� ��� �浹 ������ ���� ����.
 * �� Ŭ������ ������ apex(������ ��ġ)�� ��������, ������ ����(ConeDirection)���� ConeRange ���� ������,
 * ConeAngle (��ü ����) �̳��� �ִ� ������ �浹 ������ ����
 */
UCLASS()
class PROJECT_S_API ASkill2DConeCollisionActor : public ASkill2DCollisionActor
{
	GENERATED_BODY()
	
public:
    ASkill2DConeCollisionActor();

    // ���� �浹 ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision|Cone")
    float ConeRange;

    // ���� ��ü ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision|Cone")
    float ConeAngle;

    // ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision|Cone")
    FVector ConeDirection;

    bool Bso = true;

protected:
    // �θ� ���� �����Լ��� �������̵��Ͽ� ���� �浹 ���� �ʱ�ȭ
    virtual void InitializeCollision(const FSkillData& InSkillData) override;

    // ����� �ð�ȭ
    virtual void DrawCollisionDebug() override;

public:
    virtual void Tick(float DeltaTime) override;

};
