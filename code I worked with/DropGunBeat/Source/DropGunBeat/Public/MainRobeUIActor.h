// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainRobeUIActor.generated.h"

UCLASS()
class DROPGUNBEAT_API AMainRobeUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMainRobeUIActor();

	UPROPERTY()
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category = "Widget")
	class UMainRobeUserWidget* MainRobeWidget;

	UPROPERTY(VisibleAnywhere, Category="Widget")
	class UWidgetComponent* MainUIComponent;

	UPROPERTY(EditAnywhere, Category = "Play")
	bool bLevel = false;

	void MoveLevel();

	class UmusicGameInstance* gi = nullptr;

protected:
	virtual void BeginPlay() override;


	

};
