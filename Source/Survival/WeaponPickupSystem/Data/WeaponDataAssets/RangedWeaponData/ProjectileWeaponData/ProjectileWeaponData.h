// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RangedWeaponData.h"
#include "ProjectileWeaponData.generated.h"

class UProjectileData;
class AProjectile;

/* TODO: Bu sınıfı Raycast ile birleştir ortak bir isim koy. */

USTRUCT(BlueprintType)
struct FProjectileWeaponEffects
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "WeaponEffects")
	UParticleSystem* FrontMuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* TracerEffect;
	
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* RearMuzzleEffect;
	
};

UCLASS(BlueprintType, Blueprintable)
class SURVIVAL_API UProjectileWeaponData : public URangedWeaponData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile | Effects")
	FProjectileWeaponEffects ProjectileWeaponEffects;
	
	// UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	// TObjectPtr<UProjectileData> ProjectileData;
	
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon")
	UAnimMontage* WeaponAnimMontages;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileFireSpeed;
};
