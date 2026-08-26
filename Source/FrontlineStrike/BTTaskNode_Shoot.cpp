// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Shoot.h"

#include "ShooterAI.h"

UBTTaskNode_Shoot::UBTTaskNode_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTaskNode_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AShooterAI* OwnerController = Cast<AShooterAI>(OwnerComp.GetOwner());
	if (OwnerController)
	{
		AFrontlineStrikeCharacter* Player = OwnerController->PlayerCharactor;
		AFrontlineStrikeCharacter* Owner = OwnerController->MyCharactor;
		if (Player && Owner && Player->IsAlive)
		{
			Owner->StartShoot();
			Owner->StartShoot();
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
