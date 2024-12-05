// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UResourceWidget::UpdateHealthBar(float NewHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(NewHealth / MaxHealth);
	}

	if (HealthText)
	{
		const FString HealthString = FString::Printf(TEXT("%.0f / %.0f"), NewHealth, MaxHealth);
		HealthText->SetText(FText::FromString(HealthString));
	}
}

void UResourceWidget::UpdateManaBar(float NewMana, float MaxMana)
{
	if (ManaBar)
	{
		ManaBar->SetPercent(NewMana / MaxMana);
	}

	if (ManaText)
	{
		const FString ManaString = FString::Printf(TEXT("%.0f / %.0f"), NewMana, MaxMana);
		ManaText->SetText(FText::FromString(ManaString));
	}
}

void UResourceWidget::UpdateStaminaBar(float NewStamina, float MaxStamina)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(NewStamina / MaxStamina);
	}

	if (StaminaText)
	{
		const FString StaminaString = FString::Printf(TEXT("%.0f / %.0f"), NewStamina, MaxStamina);
		StaminaText->SetText(FText::FromString(StaminaString));
	}
}
