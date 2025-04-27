// Fill out your copyright notice in the Description page of Project Settings.


#include "gameStartActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include "playStartWidget.h"


AgameStartActor::AgameStartActor()
{
	PrimaryActorTick.bCanEverTick = false;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	sceneComp->SetupAttachment(RootComponent);


	startComponent = CreateDefaultSubobject <UWidgetComponent>(TEXT("Player Gun Component"));
	startComponent->SetupAttachment(sceneComp);

}


void AgameStartActor::BeginPlay()
{
	Super::BeginPlay();
	
	StartWidget = Cast<UplayStartWidget>(startComponent->GetWidget());

}



