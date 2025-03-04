// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSwitchCooldownWidget.h"

#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"

void UWeaponSwitchCooldownWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CooldownProgressBar)
	{
		CooldownProgressBar->SetPercent(0.0f);
	}
}

void UWeaponSwitchCooldownWidget::StartCooldown() const
{
	if (CooldownProgressBar)
	{
		CooldownProgressBar->SetPercent(1.0f);
	}
}

void UWeaponSwitchCooldownWidget::UpdateCooldownProgress(float RemainingTime, float TotalTime) const
{
	if (!CooldownProgressBar)
	{
		return;
	}
	
	float Progress = RemainingTime / TotalTime;
	CooldownProgressBar->SetPercent(Progress);
	
	if (RemainingTime <= 0.1f)
	{
		CooldownProgressBar->SetPercent(0.0f);
	}
}

