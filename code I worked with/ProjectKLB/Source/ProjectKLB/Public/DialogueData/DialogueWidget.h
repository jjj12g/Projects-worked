// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

/**
 * * NPC와 대화할 때 표시되는 위젯 (대화 및 호감도 증가 버튼 포함)
 */

 class UButton;
 class UWidgetSwitcher;
 class UTextBlock;
 class AFavorabilityBaseNpcCharacter;
 class AVillageCharacter;
 class UBorder;


UCLASS()
class PROJECTKLB_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // NPC 참조 
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
    AFavorabilityBaseNpcCharacter* NPCRef;
	
    // 대화 텍스트를 표시할 위젯 스위처
    UPROPERTY(meta = (BindWidget))
    UWidgetSwitcher* DialogueSwitcher;

    // 기본 대사 텍스트
    UPROPERTY(meta = (BindWidget))
    UTextBlock* DialogueTextDefault;

    // 업데이트된 대사 텍스트 
    UPROPERTY(meta = (BindWidget))
    UTextBlock* DialogueTextUpdated;

    UPROPERTY(meta = (BindWidgetOptional))
    UBorder* CheckButtonBorder;

    AVillageCharacter* APlayer;

    // 대사 텍스트를 업데이트하는 함수
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void UpdateDialogueText(const FText& NewDialogue);

protected:
    // 위젯이 생성된 후 초기화를 위한 함수
    virtual void NativeConstruct() override;

    // 호감도 증가 버튼의 클릭 이벤트를 처리할 함수
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void OnFavorabilityButtonClicked();

    // 확인버튼
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void OnCheckButtonClicked();

    // 호감도 증가 버튼의 클릭 이벤트를 처리할 함수
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void OnCheckButtonHovered();

    // 호감도 증가 버튼의 클릭 이벤트를 처리할 함수
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void OnCheckButtonUnhovered();

    // 호감도버튼
    UPROPERTY(meta = (BindWidget))
    UButton* FavorabilityButton;

    // 확인버튼
    UPROPERTY(meta = (BindWidget))
    UButton* CheckButton;

};
