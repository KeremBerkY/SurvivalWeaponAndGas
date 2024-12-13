// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"


class AWeaponPickup;

UENUM(BlueprintType)
enum class EWeaponCategoriesData : uint8
{
	EWCD_RaycastWeapons UMETA(DisplayName = "RaycastWeapons"),
	EWCD_ProjectileWeapons UMETA(DisplayName = "ProjectileWeapons"),
	EWCD_MeleeWeapons UMETA(DisplayName = "MeleeWeapons"),
	EWCD_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EWeaponTypeData : uint8
{
	// Raycast Silahlar
	Ewtd_Unarmed UMETA(DisplayName = "Unarmed"),
	Ewtd_AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	Ewtd_Pistol UMETA(DisplayName = "Pistol"),
	Ewtd_Shotgun UMETA(DisplayName = "Shotgun"),

	// Projectile Silahlar
	EWTd_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"),
	Ewtd_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),

	// Melee Silahlar
	EWTd_Sword UMETA(DisplayName = "Sword"),
	EWTd_Axe UMETA(DisplayName = "Axe"),

	// Diğer türler için
	EWTd_None UMETA(DisplayName = "None"),
	EWTd_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FWeaponAttributesData // AmmoCapacity total Ammo buna bir üst limitte koy bir yere kadar alınabilsin. AmmoMagazine de şarjör miktarı. AmmoMagazine biterse AmmoCapacityden AmmoMagazine kadar veya kalan kadar alsın ve yenilemeleri yap.
{
	GENERATED_USTRUCT_BODY()

	FWeaponAttributesData() : WeaponName(TEXT("")), WeaponCategories(EWeaponCategoriesData::EWCD_MAX),
							  WeaponType(EWeaponTypeData::EWTd_None), Damage(0),
							  WeaponImage(nullptr), WeaponMesh(nullptr)
	{
	}; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponCategoriesData WeaponCategories;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponTypeData WeaponType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* WeaponImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;

	// // TODO: SocketName'leri Silahı yaratırken tanımla!! Daha Sonra GetSocketName() Fonksiyonundan kullanılan socketlere ulaş
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	// FName SocketName;

	FORCEINLINE FString GetWeaponName() const { return WeaponName; }
	FORCEINLINE EWeaponCategoriesData GetWeaponCategory() const { return WeaponCategories; }
	FORCEINLINE EWeaponTypeData GetWeaponType() const { return WeaponType; }
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE UTexture2D* GetWeaponImage() const { return WeaponImage; }
	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

};


UCLASS()
class SURVIVAL_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Attributes")
	FWeaponAttributesData WeaponAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Pickup")
	TSubclassOf<AWeaponPickup> WeaponPickupClass;

	
};
