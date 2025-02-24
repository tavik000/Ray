// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserBase.h"

#include "Ray/Character/RayCharacter.h"


ALaserBase::ALaserBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	AActor::SetReplicateMovement(true);
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	LaserEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserEffect"));
	LaserEffect->SetupAttachment(SceneComponent);
	LaserCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	LaserCollision->SetupAttachment(LaserEffect);
	LaserCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	LaserCollision->OnComponentBeginOverlap.AddDynamic(this, &ALaserBase::OnBoxComponentBeginOverlap);
}

void ALaserBase::BeginPlay()
{
	Super::BeginPlay();
}

void ALaserBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		const FVector NewLocation = GetActorLocation() + MoveDirection * MoveSpeed * DeltaTime;
		SetActorLocation(NewLocation);
	}
}

void ALaserBase::MulticastInit_Implementation(FVector4 Color, FVector NewMoveDirection, float NewMoveSpeed)
{
	if (!IsValid(LaserEffect))
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Set Color"));

	LaserEffect->SetVariableVec4("Color", Color);
	MoveDirection = NewMoveDirection;
	MoveSpeed = NewMoveSpeed;
}

void ALaserBase::OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                            const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}

	ARayCharacter* Character = Cast<ARayCharacter>(OtherActor);
	if (!IsValid(Character))
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Character Overlap"));
}

