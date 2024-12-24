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

	FORCEINLINE URaycastWeaponData* GetRaycastWeaponData() const { return RaycastWeaponData; }

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Fire() override;
	virtual void EndFire() override;

	void BurstFire();
	void ResetFire() const;
	void BurstFireCooldown();

	UPROPERTY()
	ARaycastWeapons* OwnerWeapon;

	FTimerHandle BurstFireTimerHandle;
	FTimerHandle BurstFireLoopTimerHandle;
	FTimerHandle BurstCoolDownTimerHandle;
	FTimerHandle FireRateTimerHandle;

		
private:
	
	UPROPERTY()
	ARaycastWeapons* RaycastWeapon;
	
	UPROPERTY()
	URaycastWeaponData* RaycastWeaponData;

	UPROPERTY(EditAnywhere, Category = "BurstShots")
	float BurstCount;
	UPROPERTY(EditAnywhere, Category = "BurstShots")
	float BurstShotRemaining;
	
};
