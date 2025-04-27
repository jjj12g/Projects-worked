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


	//=============================  ����
	 
	//=============================  ���� ���� ����
	// ���� ������Ʈ (�����Ϳ��� �⺻ ������ ���̺��̳� RowName�� �Ҵ��� �� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCharacterStatComponent* StatComponent;

	// �÷��̾��� ���� RowName (��: "Warrior", "Mage" ��)
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

	void UseSkill(int32 SkillID);

	UFUNCTION()
	void CircleDebug();

	UFUNCTION()
	void SpawnSkillCollision(int32 SkillID);

	//=============================  ����
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	//=============================  ����



	//=============================  ����
protected:
	// To add mapping context
	virtual void BeginPlay();



};
