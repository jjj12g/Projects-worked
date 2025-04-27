// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "Math/UnrealMathUtility.h"
#include "GunPlayer.h"


void UPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentBullet->SetText(FText::FromString("15"));

}

void UPlayerWidget::remainBullet(int32 bulletCount)
{
	// 현재 점수에서 bulletCount만큼을 빼기
	bullet = FMath::Clamp(bullet + bulletCount, 0, 15);

	FString bulletString = FString::Printf(TEXT(" % d"), bullet);
	FText bulletText = FText::FromString(bulletString);
	CurrentBullet->SetText(bulletText);
}








