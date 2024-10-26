// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalZone.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PPGameModeBase.h"
#include "PPBall.h"

AGoalZone::AGoalZone()
{
	RootBox = CreateDefaultSubobject<UBoxComponent>("RootBox");
	RootBox->InitBoxExtent(FVector(50.f));
	RootBox->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);

	RootBox->CanCharacterStepUpOn = ECB_No;
	RootBox->SetShouldUpdatePhysicsVolume(true);
	RootBox->SetCanEverAffectNavigation(false);
	RootBox->bDynamicObstacle = true;
	RootComponent = RootBox;

	PrimaryActorTick.bCanEverTick = false;
}

void AGoalZone::BeginPlay()
{
	Super::BeginPlay();

	RootBox->OnComponentBeginOverlap.AddDynamic(this, &AGoalZone::OnGoalZoneOverlap);
}

void AGoalZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RootBox->OnComponentBeginOverlap.RemoveDynamic(this, &AGoalZone::OnGoalZoneOverlap);

	Super::EndPlay(EndPlayReason);
}

void AGoalZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGoalZone::OnGoalZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsValidLowLevel() && OtherActor->IsA(APPBall::StaticClass()))
	{
		if (const UWorld* world = GetWorld())
		{
			if (AGameModeBase* gameModeBase = UGameplayStatics::GetGameMode(world))
			{
				if (APPGameModeBase* gameMode = Cast<APPGameModeBase>(gameModeBase))
				{
					gameMode->RegisterGoal(PlayerNumber);
				}
			}
		}
	}
}