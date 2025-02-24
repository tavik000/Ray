// Fill out your copyright notice in the Description page of Project Settings.


#include "RayPlayerState.h"

#include "Net/UnrealNetwork.h"

ARayPlayerState::ARayPlayerState()
{
}

void ARayPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARayPlayerState, PlayerIndex);
}


