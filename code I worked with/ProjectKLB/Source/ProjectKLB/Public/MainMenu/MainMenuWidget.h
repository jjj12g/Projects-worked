// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */

class UButton;

UCLASS()
class PROJECTKLB_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

    // ���ӽ��� ��ư
    UPROPERTY(meta = (BindWidget))
    UButton* GameStartButton;

    // �������� ��ư
    UPROPERTY(meta = (BindWidget))
    UButton* ExitButton;

    // ���� �����Ϳ��� SettingsButton �̸��� �����ϰ� ������ �մϴ�.
    UPROPERTY(meta = (BindWidget))
    UButton* SettingsButton;

    // ���� ������ Ŭ���� Ÿ�� (Blueprint���� �Ҵ� ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class USettingsUIManager> SettingsWidgetClass;

    // ���̺� ���� ������ Blueprint Ŭ������ �����Ϳ��� �Ҵ��մϴ�.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class USaveWidget> SaveWidgetClass;


    // ������ ������ �� �ʱ�ȭ�� ���� �Լ�
    virtual void NativeConstruct() override;

    // ���� ���� �Լ�
    UFUNCTION()
    void OnGameStartButtonClicked();

    // ���� ���� �Լ�
    UFUNCTION()
    void OnExitButtonClicked();

    // ���� �Լ�
    UFUNCTION()
    void OnSettingsButtonClicked();


};
