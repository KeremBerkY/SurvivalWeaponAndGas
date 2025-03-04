// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrentWeaponWidget.generated.h"

class UMeleeCurrentWeaponWidget;
class UProjectileCurrentWeaponWidget;
class ASurvivalCharacter;
class ARaycastWeapons;
class AWeaponBase;
class UWidgetSwitcher;
class URaycastCurrentWeaponWidget;
class UTextBlock;
class URadialSliderWidget;
class UImage;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UCurrentWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	FORCEINLINE URaycastCurrentWeaponWidget* GetRaycastCurrentWeaponWidget() const { return RaycastCurrentWeapon; }
	FORCEINLINE UProjectileCurrentWeaponWidget* GetProjectileCurrentWeaponWidget() const { return ProjectileCurrentWeaponWidget; }
	FORCEINLINE UMeleeCurrentWeaponWidget* GetMeleeCurrentWeaponWidget() const { return MeleeCurrentWeaponWidget; }
	
	void UpdateCurrentWeaponWidget(const ASurvivalCharacter* PlayerCharacter);
	// void BindUICallbacks(AWeaponBase* WeaponBase);

	void HideCurrentWeaponWidget();

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	URaycastCurrentWeaponWidget* RaycastCurrentWeapon;

	UPROPERTY(meta = (BindWidget))
	UProjectileCurrentWeaponWidget* ProjectileCurrentWeaponWidget;
	
	UPROPERTY(meta = (BindWidget))
	UMeleeCurrentWeaponWidget* MeleeCurrentWeaponWidget;

};
