// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class SURVIVAL_API AMeleeWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	AMeleeWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
