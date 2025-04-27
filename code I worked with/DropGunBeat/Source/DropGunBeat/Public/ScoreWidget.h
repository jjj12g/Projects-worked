// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class DROPGUNBEAT_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere ,Category = "MySettings", meta = (BindWidget))
	class UTextBlock* CurrentScore;

	UPROPERTY(EditAnywhere ,Category = "MySettings", meta = (BindWidget))
	class UTextBlock* BBKKBastScore;

	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* BBKKimage;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* NightBastScore;

	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* Nightimage;

	void SetScore();
	// bbkk
	void BBKKScore();
	void SaveBBKKBestScore();
	// night
	void NightScore();
	void SaveNightBestScore();

	UPROPERTY()
	class AGunPlayer* player;

	void NativeConstruct(); 
	
private:
	int32 bestBBKKPoint = 0;

	int32 bestNightScore = 0;

};
