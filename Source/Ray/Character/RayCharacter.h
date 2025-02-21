// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "RayCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ARayCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARayCharacter();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);


	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void SendLaserTop(const FInputActionValue& Value);
	void SendLaserCenter(const FInputActionValue& Value);
	void SendLaserBottom(const FInputActionValue& Value);
	void SendLaserLeft(const FInputActionValue& Value);
	void SendLaserMiddle(const FInputActionValue& Value);
	void SendLaserRight(const FInputActionValue& Value);

	void SpawnLaser(FVector SpawnLocation, FRotator SpawnRotation);

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SendRayTopAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SendRayCenterAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SendRayBottomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SendRayLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SendRayMiddleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SendRayRightAction;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Laser)
	TSubclassOf<class ALaserBase> LaserClass;

	UPROPERTY(EditDefaultsOnly, Category = Laser)
	float LaserSpeed;

	// 0 for player 1, 1 for player 2
	UPROPERTY(EditDefaultsOnly, Category = Laser)
	TArray<FVector> TopLaserSpawnLocations;

	UPROPERTY(EditDefaultsOnly, Category = Laser)
	TArray<FVector> CenterLaserSpawnLocations;

	UPROPERTY(EditDefaultsOnly, Category = Laser)
	TArray<FVector> BottomLaserSpawnLocations;

	UPROPERTY(EditDefaultsOnly, Category = Laser)
	TArray<FVector> LeftLaserSpawnLocations;

	UPROPERTY(EditDefaultsOnly, Category = Laser)
	TArray<FVector> RightLaserSpawnLocations;

	UPROPERTY(EditAnywhere)
	int32 PlayerIndex;
	
};
