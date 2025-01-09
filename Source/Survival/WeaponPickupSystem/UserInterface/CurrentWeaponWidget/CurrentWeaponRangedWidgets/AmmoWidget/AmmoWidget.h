// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"

class ARangedWeapon;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindUICallbacks(ARangedWeapon* RangedWeapon);

	UFUNCTION()
	void UpdateAmmo(int32 CurrentAmmo, int32 AmmoCapacity);
	void SetCurrentAmmo(int32 Ammo) const;
	void SetTotalAmmo(int32 Ammo) const;
	
private:
	UPROPERTY(meta = (BindWidget))
	UImage* AmmoImage; // Ammo biterse Mermi resmini kırmızı yap!
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentAmmoText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalAmmoText;
};
