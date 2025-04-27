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

	// 시작되었을때 gi->true면 bbkk
	if (GI->bIsPlaingBBKK)
	{
		// 파일에 있는 최고 점수 값을 읽어서 bestPoint 변수에 넣는다.
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

		// bestPoint 변수의 값을 출력한다.
		BBKKBastScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), GI->bestBBKKScore)));
	}
	// 아니면 false -> 나이트시어터
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
	// 최고 점수를 파일(.txt)로 저장한다.
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
	// 최고 점수를 파일(.txt)로 저장한다.
	UmusicGameInstance* GI = Cast <UmusicGameInstance>(GetGameInstance());
	FString savePoint = FString::Printf(TEXT("%d"), GI->bestNightScore);
	FString fullPath = FPaths::ProjectContentDir() + FString("/Save/BestnightScore.txt");

	FFileHelper::SaveStringToFile(savePoint, *fullPath);
}