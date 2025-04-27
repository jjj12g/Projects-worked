// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VillageController.generated.h"

/**
 * 
 */




UCLASS()
class PROJECTKLB_API AVillageController : public APlayerController
{
	GENERATED_BODY()


public:
    AVillageController();

    /** BeginPlay 호출 시 초기화 */
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;
  
    void GameOnlyInputMode();
  
    void GameAndUIInputMode();

};

	

