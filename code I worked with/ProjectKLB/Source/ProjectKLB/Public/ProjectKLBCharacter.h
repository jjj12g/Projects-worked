// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterStat/CharacterStat.h"
#include "Inventory/ItemData.h" // 아이템 데이터 구조체 헤더 포함
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


	// 캐릭터 스탯 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	UCharacterStatComponent* StatComponent;

	// 스탯 초기화 함수
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void InitializeStats();

	// 플레이어 스탯 업데이트 함수
	void UpdatePlayerStatsFromComponent();

	// 플레이어의 현재 스탯
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	FCharacterStat CharacterStats;

	// 플레이어의 이름
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	FName PlayerName;

	// AttackMontage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* PlayerAttackMontage;

	// 플레이어 데미지 받는 함수
	UFUNCTION(BlueprintCallable, Category = "Combat") 
	void ApplyDamage(int32 DamageAmount);


	// 플레이어 HP 바 Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UHealthBarWidget* HealthWidget;

	// 현재 체력
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


	// 체력 초기화 및 UI 업데이트
	void UpdateHealthBar();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	ATile* CurrentTile = nullptr;


	// 캐릭터 아이콘 텍스처
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* CharacterIcon;

	// --------------------- 플레이어 장비창
	// 각 장비 슬롯 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FItemData Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FItemData Chest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FItemData Legs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	FItemData Helmet;

	// 장비 슬롯을 업데이트하는 함수 선언
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void EquipItemInSlot(EEquipmentType SlotType, const FItemData& NewItem);


	// --------------------- 플레이어 장비창

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};

