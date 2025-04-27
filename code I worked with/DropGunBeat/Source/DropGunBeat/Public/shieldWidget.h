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

	// ½¯µå
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* shieldPS;

	// ½¯µåÆÄ±«
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* bshieldPS;

	// Á¡¼ö ¹è°æ
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* NumberRad;

	// X¹è°æ
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* XRad;

	// ½¯µå±úÁ³À»¶§ ¼ýÀÚ
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* CurrentShield;

	// ¹èÀ²
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* NumberX;

	// X ÅØ½ºÆ®
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
