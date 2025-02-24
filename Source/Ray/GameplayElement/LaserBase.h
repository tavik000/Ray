// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "LaserBase.generated.h"

UCLASS()
class RAY_API ALaserBase : public AActor
{
	GENERATED_BODY()

public:
	ALaserBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                const FHitResult& SweepResult);


	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> LaserEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> LaserCollision;

	UPROPERTY(EditAnywhere)
	FVector MoveDirection = FVector(90.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 3.f;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastInit(FVector4 Color, FVector NewMoveDirection, float NewMoveSpeed);
};
