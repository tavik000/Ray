// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RayPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RAY_API ARayPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ARayPlayerState();

protected:

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	int32 GetPlayerIndex() const { return PlayerIndex; }

	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 PlayerIndex;
};
