// Copyright Epic Games, Inc. All Rights Reserved.

#include "RayCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Ray/GameplayElement/LaserBase.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ARayCharacter

ARayCharacter::ARayCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARayCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ARayCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARayCharacter::Move);

		EnhancedInputComponent->BindAction(SendRayTopAction, ETriggerEvent::Started, this,
		                                   &ARayCharacter::SendLaserTop);

		EnhancedInputComponent->BindAction(SendRayCenterAction, ETriggerEvent::Started, this,
		                                   &ARayCharacter::SendLaserCenter);

		EnhancedInputComponent->BindAction(SendRayBottomAction, ETriggerEvent::Started, this,
		                                   &ARayCharacter::SendLaserBottom);

		EnhancedInputComponent->BindAction(SendRayLeftAction, ETriggerEvent::Started, this,
		                                   &ARayCharacter::SendLaserLeft);

		EnhancedInputComponent->BindAction(SendRayMiddleAction, ETriggerEvent::Started, this,
		                                   &ARayCharacter::SendLaserMiddle);

		EnhancedInputComponent->BindAction(SendRayRightAction, ETriggerEvent::Started, this,
		                                   &ARayCharacter::SendLaserRight);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void ARayCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const float MovementVector = Value.GetMagnitude();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(RightDirection, MovementVector);
	}
}

void ARayCharacter::SendLaserTop(const FInputActionValue& Value)
{
	SpawnLaser(TopLaserSpawnLocations[PlayerIndex], FRotator(0, 90, 0));
}

void ARayCharacter::SendLaserCenter(const FInputActionValue& Value)
{
	SpawnLaser(CenterLaserSpawnLocations[PlayerIndex], FRotator(0, 90, 0));
}

void ARayCharacter::SendLaserBottom(const FInputActionValue& Value)
{
	SpawnLaser(BottomLaserSpawnLocations[PlayerIndex], FRotator(0, 90, 0));
}

void ARayCharacter::SendLaserLeft(const FInputActionValue& Value)
{
	SpawnLaser(LeftLaserSpawnLocations[PlayerIndex], FRotator(90, 0, 0));
}

void ARayCharacter::SendLaserMiddle(const FInputActionValue& Value)
{
	SpawnLaser(CenterLaserSpawnLocations[PlayerIndex], FRotator(90, 0, 0));
}

void ARayCharacter::SendLaserRight(const FInputActionValue& Value)
{
	SpawnLaser(RightLaserSpawnLocations[PlayerIndex], FRotator(90, 0, 0));
}

void ARayCharacter::SpawnLaser(FVector SpawnLocation, FRotator SpawnRotation)
{
	// TODO: check Authority
	if (LaserClass == nullptr)
	{
		return;
	}

	UWorld* const World = GetWorld();
	if (IsValid(World))
	{
		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ALaserBase* SpawnProjectile = World->SpawnActor<ALaserBase>(
			LaserClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	}
}
