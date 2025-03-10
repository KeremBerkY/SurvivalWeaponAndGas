// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireModeBaseComponent.h"
#include "BurstShotModeComponent.generated.h"


class ARaycastWeapons;
class URaycastWeaponData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UBurstShotModeComponent : public UFireModeBaseComponent
{
	GENERATED_BODY()

public:
	UBurstShotModeComponent();

protected:
	virtual void Fire() override;
	virtual void EndFire() override;
	virtual void LoopModeFire() override;

	virtual void ResetFire() override;
	void BurstFireCooldown();

	FTimerHandle BurstFireTimerHandle;
	FTimerHandle BurstFireLoopTimerHandle;
	FTimerHandle BurstCoolDownTimerHandle;
	FTimerHandle FireRateTimerHandle;

		
private:
	
	UPROPERTY(EditAnywhere, Category = "BurstShots")
	float BurstCount;
	UPROPERTY(EditAnywhere, Category = "BurstShots")
	float BurstShotRemaining;
	
};
