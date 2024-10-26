// Fill out your copyright notice in the Description page of Project Settings.


#include "PPUtils.h"
#include "Kismet/GameplayStatics.h"

void UPPUtils::ConnectToServer(UObject* worldObject, FString serverIP, ETravelType travelType)
{
	if (UWorld* world = worldObject->GetWorld())
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(world, 0))
		{
			PlayerController->ClientTravel(serverIP, travelType);
		}
	}
}

bool UPPUtils::IsValidPlayerNumber(uint8 inPlayerNumber)
{
	return (inPlayerNumber > static_cast<uint8>(EPlayerNumbers::None) && inPlayerNumber < static_cast<uint8>(EPlayerNumbers::COUNT));
}

bool UPPUtils::IsValidPlayerNumber(EPlayerNumbers inPlayerNumber)
{
	return (inPlayerNumber > EPlayerNumbers::None && inPlayerNumber < EPlayerNumbers::COUNT);
}
