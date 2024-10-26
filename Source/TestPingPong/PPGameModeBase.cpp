// Fill out your copyright notice in the Description page of Project Settings.


#include "PPGameModeBase.h"
#include "PPBall.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "PPPlayerController.h"

void APPGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void APPGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayersLoggedIn;
	if (!bMatchStarted && NumberOfPlayersLoggedIn >= NumberOfPlayersToStartMatch)
	{
		bMatchStarted = true;
		DropTheBall();
	}
	if (APPPlayerController* playerController = Cast<APPPlayerController>(NewPlayer))
	{
		playerController->CreateUI();
	}
	UpdateUIForAllPlayers();
}

AActor* APPGameModeBase::PPFindPlayerStart(AController* Player, const FString& IncomingName)
{
	if (APPPlayerController* playerController = Cast<APPPlayerController>(Player))
	{
		EPlayerNumbers controllerPlayerNumber = playerController->GetPlayerNumber();
		if (UWorld* world = GetWorld())
		{
			for (TActorIterator<APlayerStart> It(world); It; ++It)
			{
				if (APlayerStart* playerStart = *It)
				{
					if (!(playerStart->PlayerStartTag.IsNone()))
					{
						uint8 playerNumberInt = FCString::Atoi(*playerStart->PlayerStartTag.ToString());
						if (UPPUtils::IsValidPlayerNumber(playerNumberInt))
						{
							EPlayerNumbers playerNumber = static_cast<EPlayerNumbers>(playerNumberInt);
							if (UPPUtils::IsValidPlayerNumber(controllerPlayerNumber))
							{
								if (controllerPlayerNumber == playerNumber)
								{
									return playerStart;
								}
							}
							else
							{
								if (!(UsedPlayerNumbers.Contains(playerStart->PlayerStartTag)))
								{
									playerController->SetPlayerNumber(playerNumber);
									UsedPlayerNumbers.Add(playerStart->PlayerStartTag);
									return playerStart;
								}
							}
						}
					}
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("APPGameModeBase::PPFindPlayerStart. No valid PlayerStarts available."));
	return nullptr;
}

void APPGameModeBase::DropTheBall()
{
	if (UWorld* world = GetWorld())
	{
		FVector SpawnLocation = FVector(0.0, 0.0, 92.0);
		BallOnScene = world->SpawnActor(BallAsset, &SpawnLocation, &FRotator::ZeroRotator, FActorSpawnParameters());
	}
}

void APPGameModeBase::RegisterGoal(EPlayerNumbers playerNumber)
{
	int32& currentGoaledPlayerScore = CurrentScores.FindOrAdd(playerNumber);
	++currentGoaledPlayerScore;

	BallOnScene->Destroy();
	DropTheBall();

	UpdateUIForAllPlayers();
}

void APPGameModeBase::UpdateUIForAllPlayers()
{
	if (UWorld* world = GetWorld())
	{
		for (TActorIterator<APPPlayerController> It(world); It; ++It)
		{
			if (APPPlayerController* playerController = *It)
			{
				TArray<EPlayerNumbers> currentScoresKeys;
				TArray<int32> currentScoresValues;
				for (const TPair<EPlayerNumbers, int32>& currentScore : CurrentScores)
				{
					currentScoresKeys.Add(currentScore.Key);
					currentScoresValues.Add(currentScore.Value);
				}
				playerController->UpdateUI(bMatchStarted, currentScoresKeys, currentScoresValues);
			}
		}
	}
}