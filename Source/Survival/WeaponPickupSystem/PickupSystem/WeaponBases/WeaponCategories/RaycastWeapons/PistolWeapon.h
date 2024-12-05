// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponCategories/RaycastWeapons.h"
#include "PistolWeapon.generated.h"

UCLASS()
class SURVIVAL_API APistolWeapon : public ARaycastWeapons
{
	GENERATED_BODY()

public:
	APistolWeapon();
	
	virtual void Tick(float DeltaTime) override;
	virtual void Equip(ACharacter* Character) override;
	virtual void Fire() override;
	
protected:
	virtual void BeginPlay() override;
	
	virtual void PlayWeaponEffect() override;	
};
