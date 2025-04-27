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

    // 게임시작 버튼
    UPROPERTY(meta = (BindWidget))
    UButton* GameStartButton;

    // 게임종료 버튼
    UPROPERTY(meta = (BindWidget))
    UButton* ExitButton;

    // 위젯 에디터에서 SettingsButton 이름과 동일하게 만들어야 합니다.
    UPROPERTY(meta = (BindWidget))
    UButton* SettingsButton;

    // 설정 위젯의 클래스 타입 (Blueprint에서 할당 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class USettingsUIManager> SettingsWidgetClass;

    // 세이브 슬롯 위젯의 Blueprint 클래스를 에디터에서 할당합니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class USaveWidget> SaveWidgetClass;


    // 위젯이 생성된 후 초기화를 위한 함수
    virtual void NativeConstruct() override;

    // 게임 시작 함수
    UFUNCTION()
    void OnGameStartButtonClicked();

    // 게임 종료 함수
    UFUNCTION()
    void OnExitButtonClicked();

    // 설정 함수
    UFUNCTION()
    void OnSettingsButtonClicked();


};
