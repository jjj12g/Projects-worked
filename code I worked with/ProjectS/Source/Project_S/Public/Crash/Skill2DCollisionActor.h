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

	// 하위 클래스에서 각 충돌 컴포넌트를 초기화하기 위한 가상함수
	virtual void InitializeCollision(const FSkillData& InSkillData) PURE_VIRTUAL(ASkill2DCollisionActor::InitializeCollision, );

	// 데이터 테이블로부터 읽은 스킬 데이터를 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FSkillData SkillData;


protected:
	virtual void BeginPlay() override;

	// 디버그 시각화
	virtual void DrawCollisionDebug() PURE_VIRTUAL(ASkill2DCollisionActor::DrawCollisionDebug, );

};
