// Fill out your copyright notice in the Description page of Project Settings.


#include "PPBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PPPawn.h"

// Sets default values
APPBall::APPBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSphere = CreateDefaultSubobject<USphereComponent>("RootSphere");
	RootSphere->InitSphereRadius(50.f);
	RootSphere->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	RootSphere->CanCharacterStepUpOn = ECB_No;
	RootSphere->SetShouldUpdatePhysicsVolume(true);
	RootSphere->SetCanEverAffectNavigation(false);
	RootSphere->bDynamicObstacle = true;
	RootComponent = RootSphere;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>("SphereMesh");
	SphereMesh->AttachToComponent(RootSphere, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APPBall::BeginPlay()
{
	Super::BeginPlay();
	
	RootSphere->ComponentVelocity = FVector::ForwardVector.RotateAngleAxis(FMath::RandRange(-45.f, 45.f), FVector::UpVector) * (FMath::RandBool() ? 1.f : -1.f) * StartSpeed;
	RootSphere->OnComponentHit.AddDynamic(this, &APPBall::OnBallHit);
}

void APPBall::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RootSphere->OnComponentHit.RemoveDynamic(this, &APPBall::OnBallHit);

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void APPBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RootSphere->MoveComponent((RootSphere->ComponentVelocity * DeltaTime), FRotator::ZeroRotator, true);
}

void APPBall::OnBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsValidLowLevel() && OtherActor->IsA(APPPawn::StaticClass()))
	{
		FVector bounceDirection = GetActorLocation() - OtherActor->GetActorLocation();
		bounceDirection.Z = 0;
		RootSphere->ComponentVelocity = bounceDirection.GetSafeNormal() * BounceSpeed;
	}
	else
	{
		RootSphere->ComponentVelocity = (-1.f) * RootSphere->ComponentVelocity.RotateAngleAxis(180.f, Hit.ImpactNormal).GetSafeNormal() * BounceSpeed;
	}
}