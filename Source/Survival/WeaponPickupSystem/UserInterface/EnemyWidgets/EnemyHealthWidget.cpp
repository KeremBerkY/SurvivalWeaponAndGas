// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthWidget.h"

#include "Components/ProgressBar.h"

void UEnemyHealthWidget::UpdateHealthBar(float NewHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(NewHealth / MaxHealth);
	}
}
