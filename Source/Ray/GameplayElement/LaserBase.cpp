// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserBase.h"


// Sets default values
ALaserBase::ALaserBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	AActor::SetReplicateMovement(true);
	
}

// Called when the game starts or when spawned
void ALaserBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaserBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

