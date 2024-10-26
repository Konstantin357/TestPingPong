// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerController.h"
#include "PPMatchWidget.h"
#include "Net/UnrealNetwork.h"

APPPlayerController::APPPlayerController()
{
	bReplicates = true;
	MatchUIAsset = UPPMatchWidget::StaticClass();
}

void APPPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APPPlayerController, PlayerNumber);
}

void APPPlayerController::SetPlayerNumber(EPlayerNumbers inPlayerNumber)
{
	if (PlayerNumber == EPlayerNumbers::None && UPPUtils::IsValidPlayerNumber(inPlayerNumber))
	{
		PlayerNumber = inPlayerNumber;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("APPPlayerController::SetPlayerNumbers. Trying to reset or set invalid PlayerNumber."));
	}
}

EPlayerNumbers APPPlayerController::GetPlayerNumber()
{
	return PlayerNumber;
}

void APPPlayerController::CreateUI_Implementation()
{
	if (MatchUIAsset != UUserWidget::StaticClass())
	{
		MatchUI = CreateWidget<UPPMatchWidget>(this, MatchUIAsset);
		MatchUI->AddToViewport();
	}
}

void APPPlayerController::UpdateUI_Implementation(bool bMatchStarted, const TArray<EPlayerNumbers>& currentScoresKeys, const TArray<int32>& currentScoresValues)
{
	TMap<EPlayerNumbers, int32> currentScores;
	for (int32 i = 0; i < currentScoresKeys.Num(); ++i)
	{
		currentScores.Add(currentScoresKeys[i], currentScoresValues[i]);
	}
	if (MatchUI->IsValidLowLevel())
	{
		MatchUI->UpdateMatchData(bMatchStarted, currentScores);
	}
}