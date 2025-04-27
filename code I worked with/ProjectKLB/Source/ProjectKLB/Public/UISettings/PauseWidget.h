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
    // "설정" 버튼에 대한 UPROPERTY
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

    // 셋팅창 
    UFUNCTION()
    void OnSettingsButtonClicked();

    // 게임 재게
    UFUNCTION()
    void OnResumeButtonClicked();

    // 게임 저장기능
    UFUNCTION()
    void OnSaveButtonClicked(); 

    // 메인메뉴
    UFUNCTION()
    void OnMainMenuButtonClicked();

    // 게임종료
    UFUNCTION()
    void OnExitButtonClicked();

};
