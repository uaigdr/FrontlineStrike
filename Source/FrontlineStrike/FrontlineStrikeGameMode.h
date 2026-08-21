// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FrontlineStrikeGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AFrontlineStrikeGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	virtual void BeginPlay() override;

public:
	
	/** Constructor */
	AFrontlineStrikeGameMode();

};



