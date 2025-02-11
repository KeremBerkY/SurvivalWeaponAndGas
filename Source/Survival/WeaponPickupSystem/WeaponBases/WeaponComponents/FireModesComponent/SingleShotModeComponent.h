// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireModeBaseComponent.h"
#include "Components/ActorComponent.h"
#include "SingleShotModeComponent.generated.h"


class ARangedWeapon;
class URaycastWeaponData;
class ARaycastWeapons;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API USingleShotModeComponent : public UFireModeBaseComponent
{
	GENERATED_BODY()

public:
	USingleShotModeComponent();

protected:
	virtual void Fire() override;
	virtual void EndFire() override;
	void ResetFire() const;
	
	FTimerHandle FireRateTimerHandle;
};
