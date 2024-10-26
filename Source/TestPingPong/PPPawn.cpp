// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPawn.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APPPawn::APPPawn()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	RootBox = CreateDefaultSubobject<UBoxComponent>("RootBox");
	RootBox->InitBoxExtent(FVector(10.f,150.f,92.f));
	RootBox->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	RootBox->CanCharacterStepUpOn = ECB_No;
	RootBox->SetShouldUpdatePhysicsVolume(true);
	RootBox->SetCanEverAffectNavigation(false);
	RootBox->bDynamicObstacle = true;
	RootComponent = RootBox;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;	
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->AttachToComponent(RootBox, FAttachmentTransformRules::KeepRelativeTransform);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform);

	PrimaryActorTick.bCanEverTick = true;
}

void APPPawn::BeginPlay()
{
	Super::BeginPlay();
}

void APPPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNetMode() == ENetMode::NM_Client)
	{
		if (AController* controller = GetController())
		{
			if (APlayerController* playerController = Cast<APlayerController>(controller))
			{
				FTransform currentTransform = GetActorTransform();
				ServerUpdateTransform(currentTransform);
			}
		}
	}
}

void APPPawn::PostInitProperties()
{
	Super::PostInitProperties();
}

void APPPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APPPawn::Move);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APPPawn::ServerUpdateTransform_Implementation(FTransform updatedTransform)
{
	SetActorTransform(updatedTransform);
}

void APPPawn::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}