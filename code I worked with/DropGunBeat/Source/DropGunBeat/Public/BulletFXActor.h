// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletFXActor.generated.h"

UCLASS()
class DROPGUNBEAT_API ABulletFXActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABulletFXActor();

protected:

public:	
	UPROPERTY(VisibleAnywhere, Category = "MySettings")
	class USceneComponent* rootComp;

	UPROPERTY(VisibleAnywhere, Category = "MySettings")
	class UParticleSystemComponent* particleComp;

	void PlayFX();

};
