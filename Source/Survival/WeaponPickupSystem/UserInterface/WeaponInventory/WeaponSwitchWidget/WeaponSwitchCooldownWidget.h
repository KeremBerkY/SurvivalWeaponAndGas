// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponSwitchCooldownWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UWeaponSwitchCooldownWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void StartCooldown() const;

	UFUNCTION()
	void UpdateCooldownProgress(float RemainingTime, float TotalTime) const;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* CooldownProgressBar;
};
