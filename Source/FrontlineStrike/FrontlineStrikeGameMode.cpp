// Copyright Epic Games, Inc. All Rights Reserved.

#include "FrontlineStrikeGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "FrontlineStrikeCharacter.h"
#include "ShooterAI.h"

AFrontlineStrikeGameMode::AFrontlineStrikeGameMode()
{
	// stub
}

void AFrontlineStrikeGameMode::BeginPlay()
{
	Super::BeginPlay();

	AFrontlineStrikeCharacter* Player = Cast<AFrontlineStrikeCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	TArray<AActor*> ShooterActor;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterAI::StaticClass(), ShooterActor);

	for (int i = 0; i < ShooterActor.Num(); i++)
	{
		AShooterAI* ShooterAI = Cast<AShooterAI>(ShooterActor[i]);

		if (ShooterAI)
		{
			ShooterAI->StartBehaviorTree(Player);
			UE_LOG(LogTemp, Display, TEXT("Starting BehaviorTree"));
		}
	}
}

