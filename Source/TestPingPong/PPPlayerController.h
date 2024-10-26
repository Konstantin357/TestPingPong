// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PPUtils.h"
#include "PPPlayerController.generated.h"

class UPPMatchWidget;

UCLASS()
class TESTPINGPONG_API APPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APPPlayerController();
	
private:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = "true"))
	EPlayerNumbers PlayerNumber = EPlayerNumbers::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPPMatchWidget> MatchUIAsset;

	UPROPERTY(BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	UPPMatchWidget* MatchUI;
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetPlayerNumber(EPlayerNumbers inPlayerNumber);
	EPlayerNumbers GetPlayerNumber();

	UFUNCTION(Client, Reliable)
	void CreateUI();
	UFUNCTION(Client, Reliable)
	void UpdateUI(bool bMatchStarted, const TArray<EPlayerNumbers>& currentScoresKeys, const TArray<int32>& currentScoresValues);
};
