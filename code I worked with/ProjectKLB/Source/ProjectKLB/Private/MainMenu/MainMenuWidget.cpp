// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UISettings/SettingsUIManager.h"
#include "SaveGame/SaveWidget.h"



void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 버튼이 정상적으로 바인딩되었는지 체크 후 클릭 이벤트에 델리게이트 추가
    if (GameStartButton)
    {
        GameStartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnGameStartButtonClicked);
    }

    // ExitButton이 정상적으로 바인딩되었는지 체크 후 클릭 이벤트에 델리게이트 추가
    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);
    }

    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingsButtonClicked);
    }

}

void UMainMenuWidget::OnGameStartButtonClicked()
{
    if (SaveWidgetClass)
    {
        USaveWidget* SaveWidget = CreateWidget<USaveWidget>(GetWorld(), SaveWidgetClass);
        if (SaveWidget)
        {
            SaveWidget->AddToViewport();
            // 필요시, 현재 메뉴 위젯은 화면에서 제거할 수 있습니다.
            RemoveFromParent();
        }
    }
}

void UMainMenuWidget::OnExitButtonClicked()
{
    // GetOwningPlayer()를 통해 현재 플레이어 컨트롤러를 가져온 후, QuitGame 함수 호출
    if (APlayerController* PC = GetOwningPlayer())
    {
        UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
    }
}

void UMainMenuWidget::OnSettingsButtonClicked()
{
    if (SettingsWidgetClass)
    {
        // SettingsWidgetClass는 에디터에서 WBP_SettingsWidget 등으로 지정합니다.
        USettingsUIManager* SettingsWidget = CreateWidget<USettingsUIManager>(GetWorld(), SettingsWidgetClass);
        if (SettingsWidget)
        {
            // 설정 위젯을 화면에 표시
            SettingsWidget->AddToViewport();

            // 현재 위젯을 제거하여 보이지 않게 함
            SetVisibility(ESlateVisibility::Hidden);
        }
    }
    

}


