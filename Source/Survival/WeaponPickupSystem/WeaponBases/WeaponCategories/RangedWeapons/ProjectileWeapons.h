// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RangedWeapon.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "ProjectileWeapons.generated.h"


class UProjectileEffectManagerComponent;
class UProjectileWeaponData;
class UProjectileData;
class AProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileEffectsInit); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileFireMade); 


UCLASS()
class SURVIVAL_API AProjectileWeapons : public ARangedWeapon
{
	GENERATED_BODY()


public:
	AProjectileWeapons();

	FOnProjectileEffectsInit OnProjectileEffectsInit;
	FOnProjectileFireMade OnProjectileFireMade;
	
	FORCEINLINE UProjectileWeaponData* GetProjectileWeaponData() const { return ProjectileWeaponData; }
	FORCEINLINE AProjectile* GetProjectile() const { return SpawnedProjectile; }
	FORCEINLINE void SetProjectile(AProjectile* NewProjectile) { SpawnedProjectile = NewProjectile; }
	FORCEINLINE void SetProjectileNullptr() { SpawnedProjectile = nullptr; }
	FORCEINLINE USceneComponent* GetRearMuzzleLocation() const { return RearMuzzleLocation; }
	FORCEINLINE UProjectileEffectManagerComponent* GetEffectManagerComponent() const { return EffectManagerComponent; }
	
	// UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	// TSubclassOf<AProjectile> ProjectileClass;
	
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	// float ProjectileSpeed;
	
	// UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")
	// UParticleSystem* MuzzleFlashEffect;
	//
	// UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")
	// UParticleSystem* TracerEffect;
	
	// UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")
	// UParticleSystem* RearMuzzleEffect;
	


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void PerformFire() override;
	virtual void Attack() override;

	// virtual void PlayWeaponEffect() override;
	// FRecoilAttributes RecoilSettings;
	
private:
	void ResetFire();
	
	UPROPERTY(EditAnywhere, Category = "Weapon | Effects")
	float TrailEffectDistance;

	UPROPERTY()
	UProjectileWeaponData* ProjectileWeaponData;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USceneComponent* RearMuzzleLocation;
	
	UPROPERTY()
	UProjectileEffectManagerComponent* EffectManagerComponent;

	UPROPERTY()
	AProjectile* SpawnedProjectile;

	FTimerHandle FireRateTimerHandle;

};
