// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPUtils.h"
#include "GoalZone.generated.h"

class UBoxComponent;
//struct FHitResult;

UCLASS()
class TESTPINGPONG_API AGoalZone : public AActor
{
	GENERATED_BODY()
	
public:
	AGoalZone();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GoalZone, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RootBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GoalZone, meta = (AllowPrivateAccess = "true"))
	EPlayerNumbers PlayerNumber = EPlayerNumbers::None;

	UFUNCTION()
	void OnGoalZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};