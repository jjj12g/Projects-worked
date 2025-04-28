// Fill out your copyright notice in the Description page of Project Settings.


#include "UISettings/PauseWidget.h"
#include "Components/Button.h"
#include "ProjectKLBPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "HUD/UIHUD.h"
#include "Kismet/GameplayStatics.h"         
#include "Kismet/KismetSystemLibrary.h"     
#include "UObject/NameTypes.h"              



void UPauseWidget::NativeConstruct()
{
    Super::NativeConstruct();


    // 버튼 이벤트 바인딩
    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &UPauseWidget::OnSettingsButtonClicked);
    }

    if (ResumeButton)
    {
        ResumeButton->OnClicked.AddDynamic(this, &UPauseWidget::OnResumeButtonClicked);
    }

    if (SaveButton)
    {
        SaveButton->OnClicked.AddDynamic(this, &UPauseWidget::OnSaveButtonClicked);
    }

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &UPauseWidget::OnMainMenuButtonClicked);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UPauseWidget::OnExitButtonClicked);
    }

}



void UPauseWidget::OnResumeButtonClicked()
{
    APlayerController* OwningPlayer = GetOwningPlayer();
    if (OwningPlayer)
    {        
        OwningPlayer->SetPause(false);

        this->SetVisibility(ESlateVisibility::Collapsed);

       
        UE_LOG(LogTemp, Warning, TEXT("Resume button clicked"));
    }
}

void UPauseWidget::OnSaveButtonClicked()
{
    // 게임 저장 기능 호출
    APlayerController* OwningPlayer = GetOwningPlayer();
    if (OwningPlayer)
    {
        AProjectKLBPlayerController* PC = Cast<AProjectKLBPlayerController>(OwningPlayer);
        if (PC)
        {
            //PC->SaveGame(); // 저장 함수는 별도로 정의 필요
        }
    }
}

void UPauseWidget::OnSettingsButtonClicked()
{
    this->SetVisibility(ESlateVisibility::Collapsed);

    // 플레이어 컨트롤러 가져오기
    APlayerController* OwningPlayer = GetOwningPlayer();
    if (OwningPlayer)
    {
        // CustomPlayerController로 캐스팅
        AProjectKLBPlayerController* PC = Cast<AProjectKLBPlayerController>(OwningPlayer);
        if (PC)
        {
            // HUD 가져오기
            AUIHUD* HUD = Cast<AUIHUD>(PC->GetHUD());
            if (HUD)
            {
                // Settings UI 표시
                HUD->ShowSettingsMenu();
            }
        }
    }
}

void UPauseWidget::OnMainMenuButtonClicked()
{
    // 메인 메뉴로 이동
    UGameplayStatics::OpenLevel(this, FName("MainMenu")); // MainMenu는 레벨 이름
}

void UPauseWidget::OnExitButtonClicked()
{
    // 게임 종료
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
