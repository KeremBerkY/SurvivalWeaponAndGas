// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/FireModesComponent/FireModeBaseComponent.h"
#include "WeaponFireModesWidget.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class UImage;

/**
 * 
 */
UCLASS()
class SURVIVAL_API UWeaponFireModesWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	// void SetFireModeState(int32 FireModeIndex);
	void SetWidgetVisibility(bool bVisible);
	void SetNumberOfModesText(int32 TotalModeIndex);
	void SetFireModeState(EFireMode FireMode);
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NumberOfModes;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UImage* SingleShotModeImage; // 0

	UPROPERTY(meta = (BindWidget))
	UImage* BurstShotModeImage; // 1

	UPROPERTY(meta = (BindWidget))
	UImage* AutomaticShotModeImage; // 2
	
	
};
