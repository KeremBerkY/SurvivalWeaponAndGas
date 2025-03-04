// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireModeBaseComponent.h"
#include "AutomaticShotModeComponent.generated.h"


class ARaycastWeapons;
class URaycastWeaponData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UAutomaticShotModeComponent : public UFireModeBaseComponent
{
	GENERATED_BODY()

public:
	UAutomaticShotModeComponent();

protected:
	virtual void Fire() override;
	virtual void EndFire() override;
	virtual void LoopModeFire() override;

	virtual void ResetFire() override;

	FTimerHandle AutomaticFireTimerHandle;
	FTimerHandle FireRateTimerHandle;
	
};
