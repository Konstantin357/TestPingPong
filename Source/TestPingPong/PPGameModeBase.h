// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PPUtils.h"
#include "PPGameModeBase.generated.h"

class APPBall;

UCLASS()
class TESTPINGPONG_API APPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APPBall> BallAsset = AActor::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Match, meta = (AllowPrivateAccess = "true"))
	uint8 NumberOfPlayersToStartMatch = 2;

	UPROPERTY()
	AActor* BallOnScene = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Match, meta = (AllowPrivateAccess = "true"))
	uint8 NumberOfPlayersLoggedIn = 0;

	UPROPERTY(BlueprintReadOnly, Category = Match, meta = (AllowPrivateAccess = "true"))
	bool bMatchStarted = false;

	UPROPERTY(BlueprintReadOnly, Category = Match, meta = (AllowPrivateAccess = "true"))
	TMap<EPlayerNumbers, int32> CurrentScores;

	UFUNCTION(BlueprintCallable)
	AActor* PPFindPlayerStart(AController* Player, const FString& IncomingName);

	TSet<FName> UsedPlayerNumbers;

	void UpdateUIForAllPlayers();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void DropTheBall();
	void RegisterGoal(EPlayerNumbers playerNumber);
};