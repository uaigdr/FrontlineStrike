// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"

void UHUDWidget::SetHealthBarPercent(float NewPercent)
{
	if (NewPercent >= 0 && NewPercent <= 1)
	{
		HealthBar->SetPercent(NewPercent);
	}
}
