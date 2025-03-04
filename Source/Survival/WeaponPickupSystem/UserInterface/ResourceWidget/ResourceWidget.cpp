// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Survival/WeaponPickupSystem/UserInterface/TemplateWidgets/SurvivalWidgetBase.h"

void UResourceWidget::UpdateHealthBar(float NewHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetStatusBarPercent(NewHealth / MaxHealth);
	}
}

void UResourceWidget::UpdateManaBar(float NewMana, float MaxMana)
{
	if (ManaBar)
	{
		ManaBar->SetStatusBarPercent(NewMana / MaxMana);
	}
}