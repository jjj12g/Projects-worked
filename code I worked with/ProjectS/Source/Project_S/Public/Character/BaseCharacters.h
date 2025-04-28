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
	ABaseCharacters();

	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }


	//=============================  ����
	 
	//=============================  ���� ���� ����
	// ���� ������Ʈ (�����Ϳ��� �⺻ ������ ���̺��̳� RowName�� �Ҵ��� �� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCharacterStatComponent* StatComponent;

	// �÷��̾��� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FName PlayerName;

	//=============================  ���� ���� ��

	//============================= ī�޶� ȸ�� ���� ����
	UFUNCTION()
	void OnSideRightViewPressed();

	UFUNCTION()
	void OnSideLeftViewPressed();

	// ��ǥ ī�޶� ȸ�� (���� ��ǥ)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	FRotator TargetCameraRotation;

	// ī�޶� ȸ�� ���� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraRotationInterpSpeed;

	bool bUsingKeyboardControl = true;
	
	//============================= ī�޶� ȸ�� ���� ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision2D")
	float CollisionRadius2D = 40;

	// ��ų���
	void UseSkill(int32 SkillID);

	UFUNCTION()
	void CircleDebug();

	UFUNCTION()
	void SpawnSkillCollision(int32 SkillID);

	//=============================  ���� ��
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;



protected:
	virtual void BeginPlay();



};
