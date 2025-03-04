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
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	USurvivalWidgetBase* HealthBar;

	UFUNCTION()
	void UpdateHealthBar(float NewHealth, float MaxHealth);

	UFUNCTION()
	void SetWidgetVisibility(bool SetVisible);

private:
	UFUNCTION()
	void VisibilityDelay();

	FTimerHandle BarVisibilityTimer;
	
	bool bIsFirstBroadcast;
};
