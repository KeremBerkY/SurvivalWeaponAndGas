// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponHeatBar.generated.h"

class AWeaponBase;
class ARaycastWeapons;
class ASurvivalCharacter;
class UProgressBar;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UWeaponHeatBar : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void BindUICallbacks(ARaycastWeapons* RaycastWeapon);
protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "HeatBar")
	UProgressBar* HeatProgressBar;

	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void ShowWidget();
	UFUNCTION()
	void HideWidget();
	
	UFUNCTION()
	void UpdateHeatBar(float Heat, float MaxHeat);
	// UFUNCTION()
	// void RemoveHeat(float Heat, float MaxHeat);

private:
	ARaycastWeapons* Weapon;
	//
	// void HalfHeat();
	// void OverHeat();
};
