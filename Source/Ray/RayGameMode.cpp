// Copyright Epic Games, Inc. All Rights Reserved.

#include "RayGameMode.h"
#include "./Character/RayCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARayGameMode::ARayGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Main/Characters/Player/BP_Player_Robot"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
