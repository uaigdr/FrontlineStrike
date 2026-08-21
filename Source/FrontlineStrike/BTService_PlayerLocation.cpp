// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocation.h"

#include "ShooterAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerLocation::UBTService_PlayerLocation()
{
	NodeName = TEXT("Update PlayerLocation If Seen");
}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp,  NodeMemory, DeltaSeconds);

	AShooterAI* OwnerController = Cast<AShooterAI>(OwnerComp.GetAIOwner());
	AFrontlineStrikeCharacter* Player = OwnerController->PlayerCharactor;
	UBlackboardComponent* Blackboard = OwnerController->GetBlackboardComponent();

	if (OwnerController && Player && Blackboard)
	{
		if (OwnerController->LineOfSightTo(Player))
		{
			Blackboard->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());
			OwnerController->SetFocus(Player);
		}
		else
		{
			Blackboard->ClearValue(GetSelectedBlackboardKey());
			OwnerController->ClearFocus(EAIFocusPriority::Gameplay);
		}
	}
}
