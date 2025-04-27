// Fill out your copyright notice in the Description page of Project Settings.


#include "shieldWidget.h"
#include "Components/Image.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Animation/WidgetAnimation.h>


void UshieldWidget::startShield()
{
	//쉴드
	shieldPS->SetVisibility(ESlateVisibility::Visible);
	bshieldPS->SetVisibility(ESlateVisibility::Hidden);
	CurrentShield->SetVisibility(ESlateVisibility::Hidden);

	// 배율
	NumberRad->SetVisibility(ESlateVisibility::Visible);
	XRad->SetVisibility(ESlateVisibility::Visible);
	NumberX->SetVisibility(ESlateVisibility::Visible);
	X->SetVisibility(ESlateVisibility::Visible);

	reX = 1;
}

void UshieldWidget::hitShield()
{
	shieldPS->SetVisibility(ESlateVisibility::Hidden);
	bshieldPS->SetVisibility(ESlateVisibility::Visible);
	CurrentShield->SetVisibility(ESlateVisibility::Visible);
	CurrentShield->SetText(FText::FromString("4"));

	NumberX->SetText(FText::FromString("1"));
	reX = 1;
}

void UshieldWidget::produceShield()
{
	shieldPS->SetVisibility(ESlateVisibility::Visible);
	bshieldPS->SetVisibility(ESlateVisibility::Hidden);
	CurrentShield->SetVisibility(ESlateVisibility::Hidden);
}



void UshieldWidget::removeShield(int32 ShieldCount)
{
	// 현재 점수에서 bulletCount만큼을 빼기
	reShield = FMath::Clamp(reShield + ShieldCount, 0, 4);

	FString ShieldString = FString::Printf(TEXT("%d"), reShield);
	FText ShieldText = FText::FromString(ShieldString);
	CurrentShield->SetText(ShieldText);
}

void UshieldWidget::CurrentX(int32 _CurrentX)
{
	reX = FMath::Clamp(reX + _CurrentX, 1, 8);
	FString reXString = FString::Printf(TEXT("%d"), reX);
	FText reXText = FText::FromString(reXString);
	NumberX->SetText(reXText);
	PlayAnimationForward(anim_magnification, 2);
}