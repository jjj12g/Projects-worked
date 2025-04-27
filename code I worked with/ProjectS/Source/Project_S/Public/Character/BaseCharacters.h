// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "BaseCharacters.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UCharacterStatComponent;
class USphereComponent;

UCLASS(Blueprintable)
class PROJECT_S_API ABaseCharacters : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacters();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }


	//=============================  진원
	 
	//=============================  스탯 설정 시작
	// 스탯 컴포넌트 (에디터에서 기본 데이터 테이블이나 RowName을 할당할 수 있음)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCharacterStatComponent* StatComponent;

	// 플레이어의 스탯 RowName (예: "Warrior", "Mage" 등)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FName PlayerName;

	//=============================  스탯 설정 끝

	//============================= 카메라 회전 설정 시작
	UFUNCTION()
	void OnSideRightViewPressed();

	UFUNCTION()
	void OnSideLeftViewPressed();

	// 목표 카메라 회전 (보간 목표)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	FRotator TargetCameraRotation;

	// 카메라 회전 보간 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraRotationInterpSpeed;

	bool bUsingKeyboardControl = true;
	
	//============================= 카메라 회전 설정 끝

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision2D")
	float CollisionRadius2D = 40;

	void UseSkill(int32 SkillID);

	UFUNCTION()
	void CircleDebug();

	UFUNCTION()
	void SpawnSkillCollision(int32 SkillID);

	//=============================  진원
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	//=============================  진원



	//=============================  진원
protected:
	// To add mapping context
	virtual void BeginPlay();



};
