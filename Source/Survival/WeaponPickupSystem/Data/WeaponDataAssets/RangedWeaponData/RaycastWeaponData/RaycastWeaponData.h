// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RangedWeaponData.h"
#include "RaycastWeaponData.generated.h"

USTRUCT(BlueprintType)
struct FFiringHeatSettings
{
	GENERATED_USTRUCT_BODY()

	FFiringHeatSettings() :
			   HeatGeneratedPerShot(0),
			   MaxHeatCapacity(0),
			   HeatCooldownRate(0)
	{}
	
	// Her atışta üretilen ısı miktarı
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeatGeneratedPerShot;

	// Silahın maksimum ısı kapasitesi
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHeatCapacity;

	// Isı soğutma oranı (saniyede)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeatCooldownRate;
};

USTRUCT(BlueprintType)
struct FDecalSettings
{
	GENERATED_USTRUCT_BODY()

	FDecalSettings() :
			   DecalMaterial(nullptr),
			   DecalSize(0, 0, 0),
			   DecalLifeSpan(0)
	{}

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* DecalMaterial; // Decal için materyal

	UPROPERTY(EditDefaultsOnly)
	FVector DecalSize; // Decal boyutu

	UPROPERTY(EditDefaultsOnly)
	float DecalLifeSpan; // Decal'in sahnede kalma süresi
};

USTRUCT(BlueprintType)
struct FRaycastWeaponEffects
{
	GENERATED_USTRUCT_BODY()

	FRaycastWeaponEffects() :
		MuzzleFlashEffect(nullptr),
		TracerEffect(nullptr),
		ImpactEffect(nullptr)
	{}

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* MuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ImpactEffect;
	
};

USTRUCT(BlueprintType)
struct FWeaponAnimMontages
{
	GENERATED_USTRUCT_BODY()

	FWeaponAnimMontages() : FireMontage(nullptr) {};

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FireMontage;
	
};

UCLASS(BlueprintType, Blueprintable)
class SURVIVAL_API URaycastWeaponData : public URangedWeaponData
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
	FFiringHeatSettings FiringHeatSettings;
	
	UPROPERTY(EditAnywhere)
	FDecalSettings DecalSettings;
	
	UPROPERTY(EditAnywhere)
	FRaycastWeaponEffects WeaponEffects;
	
	UPROPERTY(EditAnywhere)
	FWeaponAnimMontages WeaponAnimMontages;
};
