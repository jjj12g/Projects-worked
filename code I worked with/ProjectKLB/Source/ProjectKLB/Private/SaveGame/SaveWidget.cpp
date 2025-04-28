// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/SaveWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/PlayerSaveGame.h"




void USaveWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton1)
        SlotButton1->OnClicked.AddDynamic(this, &USaveWidget::OnSlotButton1Clicked);
    if (SlotButton2)
        SlotButton2->OnClicked.AddDynamic(this, &USaveWidget::OnSlotButton2Clicked);
    if (SlotButton3)
        SlotButton3->OnClicked.AddDynamic(this, &USaveWidget::OnSlotButton3Clicked);
}

void USaveWidget::OnSlotButton1Clicked()
{
    LoadGameSlot(1);
}

void USaveWidget::OnSlotButton2Clicked()
{
    LoadGameSlot(2);
}

void USaveWidget::OnSlotButton3Clicked()
{
    LoadGameSlot(3);
}

void USaveWidget::LoadGameSlot(int32 SlotIndex)
{
    FString SlotName = FString::Printf(TEXT("SaveSlot%d"), SlotIndex);

    // ���� �����Ͱ� �����ϴ��� Ȯ��
    if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
    {
        // ����� �����Ͱ� ���� ��� �ε�
        UPlayerSaveGame* LoadedGame = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
        if (LoadedGame)
        {
            // ����� �����Ϳ� �ִ� ������ �̵�
            FName LevelToLoad = FName(*LoadedGame->SavedLevelName);
            UGameplayStatics::OpenLevel(GetWorld(), LevelToLoad);
        }
    }
    else
    {
        // ���� �����Ͱ� ������ ù �������� �����ϰ� �� ���� ������ �̵�
        UGameplayStatics::OpenLevel(GetWorld(), TEXT("testMap2"));
    }
}
