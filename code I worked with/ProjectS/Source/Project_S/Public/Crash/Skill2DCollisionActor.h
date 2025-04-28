// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillCollection/SkillData.h"
#include "Skill2DCollisionActor.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT_S_API ASkill2DCollisionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkill2DCollisionActor();

	virtual void Tick(float DeltaTime) override;

	// ���� Ŭ�������� �� �浹 ������Ʈ�� �ʱ�ȭ�ϱ� ���� �����Լ�
	virtual void InitializeCollision(const FSkillData& InSkillData) PURE_VIRTUAL(ASkill2DCollisionActor::InitializeCollision, );

	// ������ ���̺�κ��� ���� ��ų �����͸� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FSkillData SkillData;


protected:
	virtual void BeginPlay() override;

	// ����� �ð�ȭ
	virtual void DrawCollisionDebug() PURE_VIRTUAL(ASkill2DCollisionActor::DrawCollisionDebug, );

};
