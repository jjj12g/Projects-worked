// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizeActor.h"
#include "CustomizeWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>



ACustomizeActor::ACustomizeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	sceneComp->SetupAttachment(RootComponent);

	RobeCustomComponent = CreateDefaultSubobject <UWidgetComponent>(TEXT("Player Gun Component"));
	RobeCustomComponent->SetupAttachment(sceneComp);

	OneboxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OneboxComp"));
	OneboxComp->SetupAttachment(RobeCustomComponent);

	dobleboxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("dobleboxComp"));
	dobleboxComp -> SetupAttachment(RobeCustomComponent);
}


void ACustomizeActor::BeginPlay()
{
	Super::BeginPlay();
	
	CustomWidget = Cast<UCustomizeWidget>(RobeCustomComponent->GetWidget());

}


void ACustomizeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

