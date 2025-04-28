// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterStat/CharacterStat.h"
#include "Inventory/ItemData.h" // ������ ������ ����ü ��� ����
#include "ProjectKLBCharacter.generated.h"


class UWidgetComponent;
class UCharacterStatComponent;
class UHealthBarWidget;
class ATile;
class UTexture2D;

UCLASS(Blueprintable)
class AProjectKLBCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProjectKLBCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// To add mapping context
	virtual void BeginPlay() override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }


	// ĳ���� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	UCharacterStatComponent* StatComponent;

	// ���� �ʱ�ȭ �Լ�
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void InitializeStats();

	// �÷��̾� ���� ������Ʈ �Լ�
	void UpdatePlayerStatsFromComponent();

	// �÷��̾��� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	FCharacterStat CharacterStats;

	// �÷��̾��� �̸�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	FName PlayerName;

	// AttackMontage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* PlayerAttackMontage;

	// �÷��̾� ������ �޴� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Combat") 
	void ApplyDamage(int32 DamageAmount);


	// �÷��̾� HP �� Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UHealthBarWidget* HealthWidget;

	// ���� ü��
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	float CurrentHP;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	float MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentDefense;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentSpeed;


	// ü�� �ʱ�ȭ �� UI ������Ʈ
	void UpdateHealthBar();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	ATile* CurrentTile = nullptr;


	// ĳ���� ������ �ؽ�ó
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* CharacterIcon;

	// --------------------- �÷��̾� ���â
	// �� ��� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FItemData Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FItemData Chest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FItemData Legs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FItemData Helmet;

	// ��� ������ ������Ʈ�ϴ� �Լ� ����
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void EquipItemInSlot(EEquipmentType SlotType, const FItemData& NewItem);


	// --------------------- �÷��̾� ���â

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};

