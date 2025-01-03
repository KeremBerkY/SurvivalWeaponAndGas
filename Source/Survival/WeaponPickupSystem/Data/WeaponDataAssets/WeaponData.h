// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

class AWeaponPickup;

UENUM(BlueprintType)
enum class EWeaponCategory : uint8
{
	Ewc_RaycastWeapons UMETA(DisplayName = "RaycastWeapons"),
	Ewc_ProjectileWeapons UMETA(DisplayName = "ProjectileWeapons"),
	Ewc_MeleeWeapons UMETA(DisplayName = "MeleeWeapons"),
	Ewc_Max UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EWeaponTypes : uint8
{
	// Raycast Weapons
	Ewt_Unarmed UMETA(DisplayName = "Unarmed"),
	Ewt_AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	Ewt_Pistol UMETA(DisplayName = "Pistol"),
	Ewt_Shotgun UMETA(DisplayName = "Shotgun"),

	// Projectile Weapons
	Ewt_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"),
	Ewt_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),

	// Melee Weapons
	Ewt_Sword UMETA(DisplayName = "Sword"),
	Ewt_Axe UMETA(DisplayName = "Axe"),

	// For Other Types
	Ewt_None UMETA(DisplayName = "None"),
	Ewt_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FWeaponAttributes
{
	GENERATED_USTRUCT_BODY()

	FWeaponAttributes() :
			   WeaponName(TEXT("")),
			   WeaponCategory(EWeaponCategory::Ewc_Max),
			   WeaponTypes(EWeaponTypes::Ewt_None),
			   Damage(0),
			   WeaponInventoryImage(nullptr)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponCategory WeaponCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponTypes WeaponTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UTexture2D* WeaponInventoryImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeaponPickup> WeaponPickupClass;
	
	FORCEINLINE FString GetWeaponName() const { return WeaponName; }
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE UTexture2D* GetWeaponImage() const { return WeaponInventoryImage; }

};

UCLASS(BlueprintType, Blueprintable)
class SURVIVAL_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Weapon Data")
	FWeaponAttributes WeaponAttributes;

	UPROPERTY(EditAnywhere)
	UParticleSystem* BloodEffect;
};
