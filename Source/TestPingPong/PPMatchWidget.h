// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPMatchWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTPINGPONG_API UPPMatchWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMatchData(bool bMatchStarted, const TMap<EPlayerNumbers, int32>& currentScores);
};
