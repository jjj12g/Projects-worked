// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "gameStartActor.generated.h"

UCLASS()
class DROPGUNBEAT_API AgameStartActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AgameStartActor();


	UPROPERTY()
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category = "Widget")
	class UplayStartWidget* StartWidget;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	class UWidgetComponent* startComponent;

	UPROPERTY(EditAnywhere, Category = "Play")
	bool bPlayStart = true;

	




protected:
	virtual void BeginPlay() override;

};
