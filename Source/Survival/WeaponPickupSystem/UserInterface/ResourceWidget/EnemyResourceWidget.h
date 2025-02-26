// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyResourceWidget.generated.h"

class USurvivalWidgetBase;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UEnemyResourceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	USurvivalWidgetBase* HealthBar;

	UFUNCTION()
	void UpdateHealthBar(float NewHealth, float MaxHealth);
};
