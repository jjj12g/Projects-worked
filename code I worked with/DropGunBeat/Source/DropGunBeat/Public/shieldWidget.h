// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "shieldWidget.generated.h"

/**
 * 
 */
UCLASS()
class DROPGUNBEAT_API UshieldWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class AGunPlayer* player;

	// ����
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* shieldPS;

	// �����ı�
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* bshieldPS;

	// ���� ���
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* NumberRad;

	// X���
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* XRad;

	// ����������� ����
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* CurrentShield;

	// ����
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* NumberX;

	// X �ؽ�Ʈ
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* X;

	UPROPERTY(EditAnywhere, Category="MySettings", meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* anim_magnification;



	void removeShield(int32 ShieldCount);

	void startShield();

	void hitShield();

	void produceShield();

	void CurrentX(int32 _CurrentX);

private:
	int32 reShield = 4;

	int32 reX = 4;

};
