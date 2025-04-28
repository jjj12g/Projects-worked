// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

/**
 * * NPC�� ��ȭ�� �� ǥ�õǴ� ���� (��ȭ �� ȣ���� ���� ��ư ����)
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
    // NPC ���� 
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
    AFavorabilityBaseNpcCharacter* NPCRef;
	
    // ��ȭ �ؽ�Ʈ�� ǥ���� ���� ����ó
    UPROPERTY(meta = (BindWidget))
    UWidgetSwitcher* DialogueSwitcher;

    // �⺻ ��� �ؽ�Ʈ
    UPROPERTY(meta = (BindWidget))
    UTextBlock* DialogueTextDefault;

    // ������Ʈ�� ��� �ؽ�Ʈ 
    UPROPERTY(meta = (BindWidget))
    UTextBlock* DialogueTextUpdated;

    UPROPERTY(meta = (BindWidgetOptional))
    UBorder* CheckButtonBorder;

    AVillageCharacter* APlayer;

    // ��� �ؽ�Ʈ�� ������Ʈ�ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void UpdateDialogueText(const FText& NewDialogue);

protected:
    // ������ ������ �� �ʱ�ȭ�� ���� �Լ�
    virtual void NativeConstruct() override;

    // ȣ���� ���� ��ư�� Ŭ�� �̺�Ʈ�� ó���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void OnFavorabilityButtonClicked();

    // Ȯ�ι�ư
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void OnCheckButtonClicked();

    // ȣ���� ���� ��ư�� Ŭ�� �̺�Ʈ�� ó���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void OnCheckButtonHovered();

    // ȣ���� ���� ��ư�� Ŭ�� �̺�Ʈ�� ó���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void OnCheckButtonUnhovered();

    // ȣ������ư
    UPROPERTY(meta = (BindWidget))
    UButton* FavorabilityButton;

    // Ȯ�ι�ư
    UPROPERTY(meta = (BindWidget))
    UButton* CheckButton;

};
