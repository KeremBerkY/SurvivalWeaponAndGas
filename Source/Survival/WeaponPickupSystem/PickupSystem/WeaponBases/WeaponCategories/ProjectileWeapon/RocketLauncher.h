// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/Projectile.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponCategories/ProjectileWeapons.h"
#include "RocketLauncher.generated.h"

UCLASS()
class SURVIVAL_API ARocketLauncher : public AProjectileWeapons
{
	GENERATED_BODY()

public:
	ARocketLauncher();

	virtual void Tick(float DeltaTime) override;

	virtual void Fire() override;
	virtual void Equip(ACharacter* Character) override;

protected:
	virtual void BeginPlay() override;

	// Called when the projectile needs to explode upon hitting a target
	void HandleExplosion(AProjectile* SpawnedProjectile);

	// Explosion settings specific to RocketLauncher
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Explosion | Settings")
	FExplosiveSettings ExplosiveSettings;
private:
	
};
