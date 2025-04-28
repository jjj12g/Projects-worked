// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/UIHUD.h"
#include "UISettings/SettingsUIManager.h"
#include "Blueprint/UserWidget.h"
#include "UISettings/PauseWidget.h"


void AUIHUD::BeginPlay()
{
    Super::BeginPlay();

    


}


void AUIHUD::ShowPauseMenu()
{
    if (PauseMenuWidgetClass)
    {
        //CloseActiveWidget(); // 여기서 ActiveWidget이 제대로 nullptr로 설정되었는지 확인

        PauseWidget = CreateWidget<UPauseWidget>(GetOwningPlayerController(), PauseMenuWidgetClass);
        UE_LOG(LogTemp, Warning, TEXT("Add Pause widget"));
        if (PauseWidget)
        {
            PauseWidget->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT("Pause widget port"));
            // 입력 모드 설정
            if (APlayerController* PC = GetOwningPlayerController())
            {
                FInputModeGameAndUI InputMode;
                PC->SetInputMode(InputMode);

            }
        }
    }
}



void AUIHUD::ShowSettingsMenu()
{
    if (SettingsWidgetClass)
    {
        // Settings UI 위젯 생성 및 화면에 추가
        SettingsWidget = CreateWidget<USettingsUIManager>(GetOwningPlayerController(), SettingsWidgetClass);
        if (SettingsWidget)
        {
            SettingsWidget->AddToViewport();

            // 입력 모드 설정
            APlayerController* PC = GetOwningPlayerController();
            if (PC)
            {
                FInputModeGameAndUI InputMode;
                InputMode.SetWidgetToFocus(SettingsWidget->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);
            }
        }
    }
}


