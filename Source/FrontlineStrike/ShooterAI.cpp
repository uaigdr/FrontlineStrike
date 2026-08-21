// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAI.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AShooterAI::BeginPlay()
{
	Super::BeginPlay();
	
	//RunBehaviorTree(EnemyAIBehaviorTree);
}

// Called every frame
void AShooterAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShooterAI::StartBehaviorTree(AFrontlineStrikeCharacter* Player)
{
	if (EnemyAIBehaviorTree)
	{
		MyCharactor = Cast<AFrontlineStrikeCharacter>(GetPawn());

		if (Player)
		{
			PlayerCharactor = Player;
		}

		RunBehaviorTree(EnemyAIBehaviorTree);

		UBlackboardComponent* MyBlackboard = GetBlackboardComponent();

		if (MyBlackboard && PlayerCharactor)
		{
			MyBlackboard->SetValueAsVector("StartingLocation", MyCharactor->GetActorLocation());
			MyBlackboard->SetValueAsVector("PlayerLocation", PlayerCharactor->GetActorLocation());
		}
	}
}
