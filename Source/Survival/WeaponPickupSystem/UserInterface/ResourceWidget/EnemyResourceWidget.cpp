// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyResourceWidget.h"

#include "Survival/WeaponPickupSystem/UserInterface/TemplateWidgets/SurvivalWidgetBase.h"

void UEnemyResourceWidget::UpdateHealthBar(float NewHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetStatusBarPercent(NewHealth / MaxHealth);
	}
}
