// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourceWidget.generated.h"

class USurvivalWidgetBase;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UResourceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	USurvivalWidgetBase* HealthBar;

	UPROPERTY(meta = (BindWidget))
	USurvivalWidgetBase* ManaBar;

	// UPROPERTY(meta = (BindWidget))
	// USurvivalWidgetBase* RageBar;
	
	// // Health Bar Widget
	// UPROPERTY(meta = (BindWidget))
	// UProgressBar* HealthBar;
	//
	// // Mana Bar Widget
	// UPROPERTY(meta = (BindWidget))
	// UProgressBar* ManaBar;
	//
	// // Stamina Bar Widget
	// UPROPERTY(meta = (BindWidget))
	// UProgressBar* StaminaBar;
	//
	// // Text için Health bilgisi
	// UPROPERTY(meta = (BindWidget))
	// UTextBlock* HealthText;
	//
	// // Text için Mana bilgisi
	// UPROPERTY(meta = (BindWidget))
	// UTextBlock* ManaText;
	//
	// // Text için Stamina bilgisi
	// UPROPERTY(meta = (BindWidget))
	// UTextBlock* StaminaText;

	UFUNCTION()
	void UpdateHealthBar(float NewHealth, float MaxHealth);
	UFUNCTION()
	void UpdateManaBar(float NewMana, float MaxMana);
	// UFUNCTION()
	// void UpdateRageBar(float NewRage, float MaxRage);
	//
	// UFUNCTION()
	// void SetRageBarVisibility(bool SetVisibility);
	
};
