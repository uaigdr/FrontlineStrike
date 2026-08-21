// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "FrontlineStrikeCharacter.h"

#include "ShooterAI.generated.h"

/**
 * 
 */
UCLASS()
class FRONTLINESTRIKE_API AShooterAI : public AAIController
{
	GENERATED_BODY()

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere)
	UBehaviorTree* EnemyAIBehaviorTree;

	UPROPERTY()
	AFrontlineStrikeCharacter* PlayerCharactor;

	UPROPERTY()
	AFrontlineStrikeCharacter* MyCharactor;

public:

	void StartBehaviorTree(AFrontlineStrikeCharacter* Player);
	
};
