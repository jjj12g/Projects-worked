// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialStageClearUI/TutorialStageClearUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"   // ���� ��ƿ��Ƽ ���




void UTutorialStageClearUI::NativeConstruct()
{
    // ��ư �̺�Ʈ ���ε�
    if (NextLevelButton)
    {
        NextLevelButton->OnClicked.AddDynamic(this, &UTutorialStageClearUI::OnNextLevelButtonClicked);
        UE_LOG(LogTemp, Log, TEXT("InventoryButton bound to OnInventoryButtonClicked."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryButton is null."));
    }

}

void UTutorialStageClearUI::OnNextLevelButtonClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("VillageMap"));
}
