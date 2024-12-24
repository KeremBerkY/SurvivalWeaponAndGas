// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectileData.generated.h"

class AProjectile;

USTRUCT(BlueprintType)
struct FProjectileSettings
{
	GENERATED_USTRUCT_BODY()

	FProjectileSettings() :
			   InitialSpeed(0.f),
			   DamageAmount(0.f),
			   ProjectileClass(nullptr),
			   HitEffect(nullptr),
			   ProjectileMesh(nullptr)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialSpeed; // Movement speed of the projectile
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount; // Damage amount dealt by the projectile

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* HitEffect; // Visual effect to spawn upon impact

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh; // TODO: Kaldır bunu burdan.
};

USTRUCT(BlueprintType)
struct FExplosiveSettings
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosionRadius; // Explosion radius for explosive projectiles

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosionDelay; // Delay before explosion for delayed effects
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosiveDamage; // Damage dealt by the explosion
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	bool bIsExplosive; // Is this projectile explosive
};

UCLASS(BlueprintType, Blueprintable)
class SURVIVAL_API UProjectileData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FProjectileSettings ProjectileSettings;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FExplosiveSettings ExplosiveSettings;
	
};
