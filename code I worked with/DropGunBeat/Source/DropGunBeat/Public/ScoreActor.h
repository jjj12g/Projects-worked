// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreActor.generated.h"

UCLASS()
class DROPGUNBEAT_API AScoreActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AScoreActor();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category = "Widget")
	class UScoreWidget* ScoreWidget;

	UPROPERTY(VisibleAnywhere, Category="Widget")
	class UWidgetComponent* ScoreUIComponent;
	
	class UmusicGameInstance* gi = nullptr;

protected:
	virtual void BeginPlay() override;


	



};
