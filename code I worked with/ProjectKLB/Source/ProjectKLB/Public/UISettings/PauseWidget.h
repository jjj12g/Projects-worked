// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

/**
 * 
 */

class UButton;


UCLASS()
class PROJECTKLB_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
    virtual void NativeConstruct() override;

public:
    // "����" ��ư�� ���� UPROPERTY
    UPROPERTY(meta = (BindWidget))
    UButton* ResumeButton;

    UPROPERTY(meta = (BindWidget))
    UButton* SaveButton;

    UPROPERTY(meta = (BindWidget))
    UButton* SettingsButton;

    UPROPERTY(meta = (BindWidget))
    UButton* MainMenuButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ExitButton;

    // ����â 
    UFUNCTION()
    void OnSettingsButtonClicked();

    // ���� ���
    UFUNCTION()
    void OnResumeButtonClicked();

    // ���� ������
    UFUNCTION()
    void OnSaveButtonClicked(); 

    // ���θ޴�
    UFUNCTION()
    void OnMainMenuButtonClicked();

    // ��������
    UFUNCTION()
    void OnExitButtonClicked();

};
