// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "ProjectKLBCharacter.h"
#include "Kismet/GameplayStatics.h"



void UHealthBarWidget::NativeConstruct()
{
    

}

void UHealthBarWidget::UpdateHealthBar(float HealthPercent)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(HealthPercent);
    }
    UE_LOG(LogTemp, Warning, TEXT("UpdateHealthBar Current Health : %f"), HealthPercent);
}


//void UHealthBarWidget::UpdatePlayerName(const FString& NewName)
//{
//    if (PlayerName)
//    {
//        PlayerName->SetText(FText::FromString(NewName));
//    }
//}