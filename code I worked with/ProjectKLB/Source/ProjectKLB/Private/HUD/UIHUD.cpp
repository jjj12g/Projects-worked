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
        //CloseActiveWidget(); // ���⼭ ActiveWidget�� ����� nullptr�� �����Ǿ����� Ȯ��

        PauseWidget = CreateWidget<UPauseWidget>(GetOwningPlayerController(), PauseMenuWidgetClass);
        UE_LOG(LogTemp, Warning, TEXT("Add Pause widget"));
        if (PauseWidget)
        {
            PauseWidget->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT("Pause widget port"));
            // �Է� ��� ����
            if (APlayerController* PC = GetOwningPlayerController())
            {
                FInputModeGameAndUI InputMode;
                //InputMode.SetWidgetToFocus(PauseWidget->TakeWidget()); // ��Ŀ�� ����
                //InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);

                //PC->SetPause(true); // Pause ���� ����
            }
        }
    }
}



void AUIHUD::ShowSettingsMenu()
{
    if (SettingsWidgetClass)
    {
        // Settings UI ���� ���� �� ȭ�鿡 �߰�
        SettingsWidget = CreateWidget<USettingsUIManager>(GetOwningPlayerController(), SettingsWidgetClass);
        if (SettingsWidget)
        {
            SettingsWidget->AddToViewport();

            // �Է� ��� ����
            APlayerController* PC = GetOwningPlayerController();
            if (PC)
            {
                FInputModeGameAndUI InputMode;
                InputMode.SetWidgetToFocus(SettingsWidget->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);
               // PC->bShowMouseCursor = true;
            }
        }
    }
}



//void APlayerHUD::DisplayMenu()
//{
//    if (MainManuWidget)
//    {
//        bIsMenuVisible = true;
//        MainManuWidget->SetVisibility(ESlateVisibility::Visible);
//    }
//}
//
//void APlayerHUD::HideMenu()
//{
//    if (MainManuWidget)
//    {
//        bIsMenuVisible = false;
//        MainManuWidget->SetVisibility(ESlateVisibility::Collapsed);
//    }
//}
//
//void APlayerHUD::ToggleMenu()
//{
//    if (bIsMenuVisible)
//    {
//        HideMenu();
//
//        const FInputModeGameOnly InputMode; // ����ȭ�鸸 Ŭ���ϵ��� ����
//        GetOwningPlayerController()->SetInputMode(InputMode);
//        GetOwningPlayerController()->SetShowMouseCursor(false);
//    }
//    else
//    {
//        DisplayMenu();
//
//        const FInputModeGameAndUI InputMode; // UI�� Ŭ���ϵ��� ����
//        GetOwningPlayerController()->SetInputMode(InputMode);
//        GetOwningPlayerController()->SetShowMouseCursor(true);
//    }
//}
