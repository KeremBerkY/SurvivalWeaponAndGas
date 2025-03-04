// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/WeaponData.h"
#include "RangedWeaponData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponAmmoAttributes
{
	GENERATED_USTRUCT_BODY()

	FWeaponAmmoAttributes() :
			   AmmoCapacity(0),
			   AmmoInMagazine(0),
			   CurrentAmmo(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int32 AmmoCapacity; // Taşınabilir maksimum mermi miktarı, yani karakterin üzerinde bulundurabileceği toplam mermi kapasitesi.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int32 AmmoInMagazine; // Şarjörün tam kapasitesi, yani her reload işleminde şarjöre dolan mermi miktarı

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int32 CurrentAmmo; // Şarjörde şu anda bulunan mermi miktarı.
};

USTRUCT(BlueprintType)
struct FRecoilSettings
{
	GENERATED_USTRUCT_BODY()

	FRecoilSettings() :
			   BaseRecoilAmount(0),
			   PitchMultiplier(0),
			   YawMultiplier(0),
			   RandomRecoilRange(0),
			   RecoilCooldown(0)
	{}

	UPROPERTY(EditAnywhere)
	float BaseRecoilAmount = 1.0f; // Genel geri tepme kuvveti

	UPROPERTY(EditAnywhere)
	float PitchMultiplier = 1.0f; // Yükseklik (Pitch) çarpanı

	UPROPERTY(EditAnywhere)
	float YawMultiplier = 0.2f; // Yatay (Yaw) çarpanı

	UPROPERTY(EditAnywhere)
	float RandomRecoilRange = 0.5f; // Recoil için rastgele bir seed aralığı

	UPROPERTY(EditAnywhere)
	float RecoilCooldown = 0.1f; // Recoil soğuma süresi
};



UCLASS(BlueprintType, Blueprintable)
class SURVIVAL_API URangedWeaponData : public UWeaponData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Ranged Weapon Data | AmmoAttributes")
	FWeaponAmmoAttributes WeaponAmmoAttributes;
	
	UPROPERTY(EditAnywhere, Category = "Ranged Weapon Data | FireSettings")
	float FireRate;
	
	UPROPERTY(EditAnywhere, Category = "Ranged Weapon Data | RecoilSettings")
	FRecoilSettings RecoilSettings;
	
	UPROPERTY(EditAnywhere, Category = "Ranged Weapon Data | RecoilSettings")
	UAnimMontage* ReloadAnimation;

};
