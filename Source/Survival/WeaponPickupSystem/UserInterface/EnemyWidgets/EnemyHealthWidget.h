// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UEnemyHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UFUNCTION()
	void UpdateHealthBar(float NewHealth, float MaxHealth);
};
