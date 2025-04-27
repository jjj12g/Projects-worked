// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterIconWidget.generated.h"

/**
 * 
 */

 class UUniformGridPanel;
 class UImage;

UCLASS()
class PROJECTKLB_API UCharacterIconWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    virtual void NativeConstruct() override;

    // � ĳ���� Ŭ������ ��Ÿ������ (����, �䳢 ��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
    TSubclassOf<class AProjectKLBCharacter> CharacterClass;

    // UMG���� ������ �̹����� ǥ���� UImage. �������Ʈ���� BindWidget���� ����
    UPROPERTY(meta = (BindWidget))
    UImage* IconImage;

protected:
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

};
