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

    virtual void InitializeCollision(const FSkillData& InSkillData) override;

    virtual void Tick(float DeltaTime) override;

    bool bDebugSphere = true;

protected:
    // �浹�� ���� ���Ǿ� ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    USphereComponent* CollisionSphere;

    // ����� �ð�ȭ
    virtual void DrawCollisionDebug() override;

    // �̹� �浹�� ó���� �÷��̾���� ���� (�ߺ� ó�� ����)
    TSet<ABaseCharacters*> ProcessedOverlaps;

};
