// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PPPawn.generated.h"

class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;
class UInputMappingContext;
class UInputAction;
class UInputComponent;
struct FInputActionValue;

UCLASS()
class TESTPINGPONG_API APPPawn : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RootBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

public:
	APPPawn();

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitProperties() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	UFUNCTION(Server, Reliable)
	void ServerUpdateTransform(FTransform updatedTransform);
};