// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */

 class UProgressBar;
 //class UTextBlock;
 class AProjectKLBCharacter;


UCLASS()
class PROJECTKLB_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // 체력 바 업데이트 함수
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateHealthBar(float HealthPercent);

    virtual void NativeConstruct() override;

    
    AProjectKLBCharacter* Player;
    

protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;


};
