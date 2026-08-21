// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_LastPlayerLocation.h"

#include "ShooterAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_LastPlayerLocation::UBTService_LastPlayerLocation()
{
	NodeName = TEXT("Update LastPlayerLocation");
}

void UBTService_LastPlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AShooterAI* OwnerController = Cast<AShooterAI>(OwnerComp.GetOwner());
	if (OwnerController) 
	{
		AFrontlineStrikeCharacter* Player = OwnerController->PlayerCharactor;
		UBlackboardComponent* Blackboard = OwnerController->GetBlackboardComponent();
		if (Player && Blackboard)
		{
			Blackboard->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());
		}
	}
}
