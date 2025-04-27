// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "Math/UnrealMathUtility.h"
#include "GunPlayer.h"
#include "musicGameInstance.h"
#include "Components/Image.h"


void UScoreWidget::NativeConstruct()
{
	UmusicGameInstance* GI = Cast <UmusicGameInstance>(GetGameInstance());

	SetScore();

	// ���۵Ǿ����� gi->true�� bbkk
	if (GI->bIsPlaingBBKK)
	{
		// ���Ͽ� �ִ� �ְ� ���� ���� �о bestPoint ������ �ִ´�.
		FString loadPoint;
		FString fullPath = FPaths::ProjectContentDir() + FString("/Save/BestScore.txt");
		FFileHelper::LoadFileToString(loadPoint, *fullPath);

		GI->bestBBKKScore = FCString::Atoi(*loadPoint);

		
		BBKKBastScore->SetVisibility(ESlateVisibility::Visible);
		BBKKimage->SetVisibility(ESlateVisibility::Visible);
		NightBastScore->SetVisibility(ESlateVisibility::Hidden);
		Nightimage->SetVisibility(ESlateVisibility::Hidden);

		BBKKScore();
		SaveBBKKBestScore();

		// bestPoint ������ ���� ����Ѵ�.
		BBKKBastScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), GI->bestBBKKScore)));
	}
	// �ƴϸ� false -> ����Ʈ�þ���
	else
	{
		FString loadPoint;
		FString fullPath = FPaths::ProjectContentDir() + FString("/Save/BestnightScore.txt");
		FFileHelper::LoadFileToString(loadPoint, *fullPath);
		
		GI->bestNightScore = FCString::Atoi(*loadPoint);

		NightBastScore->SetVisibility(ESlateVisibility::Visible);
		Nightimage->SetVisibility(ESlateVisibility::Visible);
		BBKKBastScore->SetVisibility(ESlateVisibility::Hidden);
		BBKKimage->SetVisibility(ESlateVisibility::Hidden);

		NightScore();
		SaveNightBestScore();

		NightBastScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), GI->bestNightScore)));
	}
	
	
	GI->currentScore = 0;

}

void UScoreWidget::SetScore()
{
	UmusicGameInstance* GI = Cast <UmusicGameInstance>(GetGameInstance());
	
	FString ScoreString = FString::Printf(TEXT("%d"), GI->currentScore);
	FText ScoreText = FText::FromString(ScoreString);
	CurrentScore->SetText(ScoreText);
}

// bbkk
void UScoreWidget::BBKKScore()
{
	UmusicGameInstance* GI = Cast <UmusicGameInstance>(GetGameInstance());

	bestBBKKPoint = GI->currentScore;
	

	if (GI->bestBBKKScore < bestBBKKPoint)
	{
		GI->bestBBKKScore = bestBBKKPoint;
	}

	FString ScoreString = FString::Printf(TEXT("%d"), GI->bestBBKKScore);
	FText ScoreText = FText::FromString(ScoreString);
	BBKKBastScore->SetText(ScoreText);
}

void UScoreWidget::SaveBBKKBestScore()
{
	// �ְ� ������ ����(.txt)�� �����Ѵ�.
	UmusicGameInstance* GI = Cast <UmusicGameInstance>(GetGameInstance());
	FString savePoint = FString::Printf(TEXT("%d"), GI->bestBBKKScore);
	FString fullPath = FPaths::ProjectContentDir() + FString("/Save/BestScore.txt");

	FFileHelper::SaveStringToFile(savePoint, *fullPath);
}

// night
void UScoreWidget::NightScore()
{
	UmusicGameInstance* GI = Cast <UmusicGameInstance>(GetGameInstance());

	bestNightScore = GI->currentScore;


	if (GI->bestNightScore < bestNightScore)
	{
		GI->bestNightScore = bestNightScore;
	}

	FString ScoreString = FString::Printf(TEXT("%d"), GI->bestNightScore);
	FText ScoreText = FText::FromString(ScoreString);
	NightBastScore->SetText(ScoreText);
}

void UScoreWidget::SaveNightBestScore()
{
	// �ְ� ������ ����(.txt)�� �����Ѵ�.
	UmusicGameInstance* GI = Cast <UmusicGameInstance>(GetGameInstance());
	FString savePoint = FString::Printf(TEXT("%d"), GI->bestNightScore);
	FString fullPath = FPaths::ProjectContentDir() + FString("/Save/BestnightScore.txt");

	FFileHelper::SaveStringToFile(savePoint, *fullPath);
}