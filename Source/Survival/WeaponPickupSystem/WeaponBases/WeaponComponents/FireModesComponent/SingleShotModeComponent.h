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

	FORCEINLINE URaycastWeaponData* GetRaycastWeaponData() const { return RaycastWeaponData; }

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// virtual void Fire() override;
	virtual void Fire() override;
	virtual void EndFire() override;
	void ResetFire() const;

	UPROPERTY()
	ARaycastWeapons* OwnerWeapon;
	// ARangedWeapon* OwnerWeapon;
	
	FTimerHandle FireRateTimerHandle;

private:
	UPROPERTY()
	URaycastWeaponData* RaycastWeaponData;
};
