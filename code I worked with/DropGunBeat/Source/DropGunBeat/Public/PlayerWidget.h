// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class DROPGUNBEAT_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere ,Category = "MySettings", meta = (BindWidget))
	class UTextBlock* CurrentBullet;

	
	UPROPERTY()
	class AGunPlayer* player;
	
	void NativeConstruct();

	void remainBullet(int32 bulletCount);


private:
	int32 bullet = 15;

};
