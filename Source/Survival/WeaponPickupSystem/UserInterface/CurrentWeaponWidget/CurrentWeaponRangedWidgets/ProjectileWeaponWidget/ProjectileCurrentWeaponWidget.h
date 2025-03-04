// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectileCurrentWeaponWidget.generated.h"

class AProjectileWeapons;
class UCurrentWeaponWidget;
class UAmmoWidget;
class UImage;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UProjectileCurrentWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetParent(UCurrentWeaponWidget* WeaponWidget);
	void BindUICallbacks(AProjectileWeapons* ProjectileWeapon);
	
	UFUNCTION()
	void InitializeAmmo();

	UFUNCTION()
	void ShowCurrentWeaponProjectileWidget();
	void HideCurrentWeaponProjectileWidget();

	void SetCurrentWeaponImage(UTexture2D* WeaponImage);
	
private:

	UPROPERTY(meta = (BindWidget))
	UImage* CurrentWeaponImage;

	UPROPERTY(meta = (BindWidget))
	UAmmoWidget* AmmoWidget;

	TWeakObjectPtr<UCurrentWeaponWidget> CurrentWeaponWidgetPtr;
	TWeakObjectPtr<AProjectileWeapons> ProjectileWeaponPtr;
};
