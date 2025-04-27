// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialStageClearUI/TutorialStageClearUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"   // 게임 유틸리티 기능




void UTutorialStageClearUI::NativeConstruct()
{
    // 버튼 이벤트 바인딩
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
