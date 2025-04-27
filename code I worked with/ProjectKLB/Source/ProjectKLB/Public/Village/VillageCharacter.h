// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "VillageCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class AVillageController;



UCLASS()
class PROJECTKLB_API AVillageCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* VillageMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;


	// ����� ���� Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MobileMappingContext;

	// ����� ��ġ �Է� �׼ǵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* TouchPressedAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* TouchReleasedAction;


public:
	AVillageCharacter();

	// To add mapping context
	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	AVillageController* AVillagePC;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	// ��ȭ ���� Ŭ���� (�������Ʈ ���� �Ҵ�)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UDialogueWidget> DialogueWidgetClass;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UVirtualJoystickWidget> VirtualJoystickWidgetClass;


	// �ε巯�� �̵��� ȸ���� ���� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Smooth Movement")
	bool bIsSmoothMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Smooth Movement")
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Smooth Movement")
	FRotator TargetRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smooth Movement")
	float MoveSpeed; // ��: 5.0f

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smooth Movement")
	float RotateSpeed; // ��: 5.0f

	// �ε巯�� �̵��� �����ϴ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Smooth Movement")
	void StartSmoothMove(const FVector& InTargetLocation, const FRotator& InTargetRotation);


protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for looking input */
	void Interact(const FInputActionValue& Value);

	void TouchPressed(const FInputActionValue& Value);
	
	void TouchReleased(const FInputActionValue& Value);

	UFUNCTION()
	void OnVirtualJoystickMoved(FVector2D MovementVector);



	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
