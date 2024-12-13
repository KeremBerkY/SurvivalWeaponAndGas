// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "RaycastWeapons.generated.h"

UCLASS()
class SURVIVAL_API ARaycastWeapons : public AWeaponBase
{
	GENERATED_BODY()

public:
	ARaycastWeapons();
	
	virtual void Tick(float DeltaTime) override;
	virtual void Fire() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")
	UParticleSystem* MuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")
	UParticleSystem* ImpactEffect;
	
protected:
	virtual void BeginPlay() override;

	virtual void PlayWeaponEffect() override;
	void PlayImpactEffect(const FHitResult& HitResult);
	virtual void ApplyRecoilEffect() override;
	
	void FireAtTarget(const FVector& Start, const FVector& End);
	void ApplyDecal(const FHitResult& HitResult);

	void DrawDebugVisuals(const FVector& Start, const FVector& End, const FHitResult& HitResult) const;
private:
	UPROPERTY(EditAnywhere, Category = "Effects")
	float TrailEffectDistance = 10000.f;
};
