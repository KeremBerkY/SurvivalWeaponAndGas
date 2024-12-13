// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/WeaponData.h"
#include "RangedWeaponData.generated.h"

USTRUCT(BlueprintType)
struct FRecoilSettingsData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Recoil Settings")
	float BaseRecoilAmount = 1.0f; // Genel geri tepme kuvveti

	UPROPERTY(EditAnywhere, Category = "Recoil Settings")
	float PitchMultiplier = 1.0f; // Yükseklik (Pitch) çarpanı

	UPROPERTY(EditAnywhere, Category = "Recoil Settings")
	float YawMultiplier = 0.2f; // Yatay (Yaw) çarpanı

	UPROPERTY(EditAnywhere, Category = "Recoil Settings")
	float RandomRecoilRange = 0.5f; // Recoil için rastgele bir seed aralığı

	UPROPERTY(EditAnywhere, Category = "Recoil Settings")
	float RecoilCooldown = 0.1f; // Recoil soğuma süresi
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon | Recoil Settings")
	bool bCanApplyRecoil = true; // Recoil'in uygulanabilir olup olmadığını kontrol eder
	
	FTimerHandle RecoilCooldownTimerHandle;
};

USTRUCT(BlueprintType)
struct FDecalSettingsData
{
	GENERATED_USTRUCT_BODY()

	FDecalSettingsData() :
			DecalMaterial(nullptr),
			DecalSize(FVector(10.0f, 10.0f, 10.0f)),
			DecalLifeSpan(10.f) {} ;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UMaterialInterface* DecalMaterial; // Decal için materyal

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	FVector DecalSize; // Decal boyutu

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float DecalLifeSpan; // Decal'in sahnede kalma süresi
};

UCLASS()
class SURVIVAL_API URangedWeaponData : public UWeaponData
{
	GENERATED_BODY()
};
