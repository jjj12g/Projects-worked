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


    // ��ư �̺�Ʈ ���ε�
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
    // ���� ���� ��� ȣ��
    APlayerController* OwningPlayer = GetOwningPlayer();
    if (OwningPlayer)
    {
        AProjectKLBPlayerController* PC = Cast<AProjectKLBPlayerController>(OwningPlayer);
        if (PC)
        {
            //PC->SaveGame(); // ���� �Լ��� ������ ���� �ʿ�
        }
    }
}

void UPauseWidget::OnSettingsButtonClicked()
{
    this->SetVisibility(ESlateVisibility::Collapsed);

    // �÷��̾� ��Ʈ�ѷ� ��������
    APlayerController* OwningPlayer = GetOwningPlayer();
    if (OwningPlayer)
    {
        // CustomPlayerController�� ĳ����
        AProjectKLBPlayerController* PC = Cast<AProjectKLBPlayerController>(OwningPlayer);
        if (PC)
        {
            // HUD ��������
            AUIHUD* HUD = Cast<AUIHUD>(PC->GetHUD());
            if (HUD)
            {
                // Settings UI ǥ��
                HUD->ShowSettingsMenu();
            }
        }
    }
}

void UPauseWidget::OnMainMenuButtonClicked()
{
    // ���� �޴��� �̵�
    UGameplayStatics::OpenLevel(this, FName("MainMenu")); // MainMenu�� ���� �̸�
}

void UPauseWidget::OnExitButtonClicked()
{
    // ���� ����
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
