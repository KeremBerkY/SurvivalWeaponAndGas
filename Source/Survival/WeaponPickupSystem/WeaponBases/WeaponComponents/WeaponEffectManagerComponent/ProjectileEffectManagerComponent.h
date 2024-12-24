// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileEffectManagerComponent.generated.h"

class UProjectileData;
class AProjectile;
class UProjectileWeaponData;
class AProjectileWeapons;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UProjectileEffectManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProjectileEffectManagerComponent();

	void InitializeProjectile(AProjectile* SpawnedProjectile);
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void PlayWeaponEffect();
	
	UFUNCTION()
	void HandleExplosion();

	UFUNCTION()
	void InitializeEffects();


private:
	UPROPERTY()	
	AProjectileWeapons* Weapon;
	
	UPROPERTY()	
	AProjectile* Projectile;
	
	UPROPERTY()
	UProjectileWeaponData* ProjectileWeaponData;
	
	UPROPERTY()	
	UProjectileData* ProjectileData;
};
