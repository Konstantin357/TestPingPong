// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PPUtils.generated.h"

UENUM(BlueprintType)
enum class EPlayerNumbers : uint8
{
	None = 0,
	One = 1,
	Two = 2,
	COUNT = 3 UMETA(Hidden)
};

UCLASS()
class TESTPINGPONG_API UPPUtils : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static void ConnectToServer(UObject* worldObject, FString serverIP, ETravelType travelType);

	static bool IsValidPlayerNumber(uint8 inPlayerNumber);
	static bool IsValidPlayerNumber(EPlayerNumbers inPlayerNumber);
};
