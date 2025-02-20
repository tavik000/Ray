// Copyright Epic Games, Inc. All Rights Reserved.

#include "RayGameMode.h"
#include "./Character/RayCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARayGameMode::ARayGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
