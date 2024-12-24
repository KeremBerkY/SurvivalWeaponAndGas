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

	FORCEINLINE URaycastWeaponData* GetRaycastWeaponData() const { return RaycastWeaponData; }


protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Fire() override;
	virtual void EndFire() override;

	void AutomaticFire();

	void ResetFire() const;


	UPROPERTY()
	ARaycastWeapons* OwnerWeapon;

	FTimerHandle AutomaticFireTimerHandle;
	FTimerHandle FireRateTimerHandle;

	
private:
	UPROPERTY()
	URaycastWeaponData* RaycastWeaponData;
	
};
