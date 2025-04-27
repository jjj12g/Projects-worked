// Fill out your copyright notice in the Description page of Project Settings.


#include "MainRobeUIActor.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "musicGameInstance.h"

// Sets default values
AMainRobeUIActor::AMainRobeUIActor()
{
	PrimaryActorTick.bCanEverTick = false;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(sceneComp);

	
	MainUIComponent = CreateDefaultSubobject <UWidgetComponent>(TEXT("Player Gun Component"));
	MainUIComponent->SetupAttachment(sceneComp);
}


void AMainRobeUIActor::MoveLevel()
{
	gi->bIsPlaingBBKK = bLevel;
	UGameplayStatics::OpenLevel(this, "LoadingLevel");
}

void AMainRobeUIActor::BeginPlay()
{
	Super::BeginPlay();
	
	MainRobeWidget = Cast<UMainRobeUserWidget>(MainUIComponent->GetWidget());

	gi = Cast<UmusicGameInstance>(GetGameInstance());
}

