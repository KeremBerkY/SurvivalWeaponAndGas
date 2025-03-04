// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MeleeCurrentWeaponWidget.generated.h"

class USurvivalWidgetBase;
class UImage;
class UCurrentWeaponWidget;
class AMeleeWeapon;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UMeleeCurrentWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void SetParent(UCurrentWeaponWidget* WeaponWidget);
	void BindUICallbacks(AMeleeWeapon* MeleeWeapon);

	UFUNCTION()
	void ShowCurrentWeaponMeleeWidget();
	void HideCurrentWeaponMeleeWidget();

	void SetCurrentWeaponImage(UTexture2D* WeaponImage);

	UFUNCTION()
	void UpdateRageBar(float NewRage, float MaxRage);

	UFUNCTION()
	void SetRageBarVisibility(bool SetVisibility);
private:

	UPROPERTY(meta = (BindWidget))
	UImage* CurrentWeaponImage;
	
	UPROPERTY(meta = (BindWidget))
	USurvivalWidgetBase* RageBar;

	TWeakObjectPtr<UCurrentWeaponWidget> CurrentWeaponWidgetPtr;
	TWeakObjectPtr<AMeleeWeapon> MeleeWeaponPtr;
};
