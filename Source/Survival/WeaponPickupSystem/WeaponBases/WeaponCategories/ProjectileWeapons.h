// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "ProjectileWeapons.generated.h"


class AProjectile;

UCLASS()
class SURVIVAL_API AProjectileWeapons : public AWeaponBase
{
	GENERATED_BODY()

public:
	AProjectileWeapons();
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float ProjectileSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")
	UParticleSystem* MuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")
	UParticleSystem* TracerEffect;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USceneComponent* RearMuzzleLocation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")
	UParticleSystem* RearMuzzleEffect;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Fire() override;
	virtual void PlayWeaponEffect() override;
	virtual void ApplyRecoilEffect() override;

	FRecoilSettings RecoilSettings;
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Weapon | Effects")
	float TrailEffectDistance;
};
