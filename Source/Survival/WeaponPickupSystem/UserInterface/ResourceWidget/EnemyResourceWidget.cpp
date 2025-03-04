// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyResourceWidget.h"

#include "Survival/WeaponPickupSystem/UserInterface/TemplateWidgets/SurvivalWidgetBase.h"

void UEnemyResourceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFirstBroadcast = true;
	SetVisibility(ESlateVisibility::Hidden);
}

void UEnemyResourceWidget::UpdateHealthBar(float NewHealth, float MaxHealth)
{
	GetWorld()->GetTimerManager().ClearTimer(BarVisibilityTimer);
	
	if (!bIsFirstBroadcast)
	{
		SetWidgetVisibility(true);
	}

	if (FMath::IsNearlyZero(NewHealth))
	{
		SetWidgetVisibility(false);
	}
	
	if (HealthBar)
	{
		HealthBar->SetStatusBarPercent(NewHealth / MaxHealth);

		GetWorld()->GetTimerManager().SetTimer(
			BarVisibilityTimer,
			this,
			&UEnemyResourceWidget::VisibilityDelay,
			5.f,
			false
		);
	}

}

void UEnemyResourceWidget::SetWidgetVisibility(bool SetVisible)
{
	if (SetVisible)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UEnemyResourceWidget::VisibilityDelay()
{
	SetWidgetVisibility(false);

	bIsFirstBroadcast = false;

}
