// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>



AScoreActor::AScoreActor()
{
	PrimaryActorTick.bCanEverTick = false;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(sceneComp);


	ScoreUIComponent = CreateDefaultSubobject <UWidgetComponent>(TEXT("Player Gun Component"));
	ScoreUIComponent->SetupAttachment(sceneComp);

}


void AScoreActor::BeginPlay()
{
	Super::BeginPlay();

	ScoreWidget = Cast<UScoreWidget>(ScoreUIComponent->GetWidget());
	

}


void AScoreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



