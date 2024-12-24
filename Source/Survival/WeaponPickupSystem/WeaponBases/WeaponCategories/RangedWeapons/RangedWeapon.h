// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "RangedWeapon.generated.h"

class UHeatComponent;
class URaycastEffectManagerComponent;
class USingleShotModeComponent;
class IWeaponFireMode;

// TODO: Bu sınıfta neler olabilir? ToggleWeaponFireMode? Reload?

UCLASS()
class SURVIVAL_API ARangedWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	ARangedWeapon();
	
	FORCEINLINE USceneComponent* GetMuzzleLocation() const { return MuzzleLocation; }
	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }
	FORCEINLINE void SetCurrentAmmo(const int32 Amount) {  CurrentAmmo = Amount;}
	FORCEINLINE void DecreaseCurrentAmmo() { CurrentAmmo--; }
	// FORCEINLINE TScriptInterface<IWeaponFireMode> GetActiveFireMode() const { return ActiveFireMode; }
	FORCEINLINE bool CanFire() const { return bCanFire; } // TODO: Belki WeaponBaseye taşıyabilirsin? 
	FORCEINLINE void SetCanFire(const bool CanFire) { bCanFire = CanFire; } // TODO: Belki WeaponBaseye taşıyabilirsin?
	
	UFUNCTION()
	virtual void Reload();
	
	UFUNCTION()
	void ToggleFireMode();
	
	virtual void PerformFire();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Attack() override;
	virtual void EndAttack() override;
	
	UFUNCTION()
	virtual void FinishReload();

private:
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	USceneComponent* MuzzleLocation;
	
	int32 CurrentAmmo;

	bool bCanFire;
	
	bool bIsReloading;

	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float ReloadDuration = 2.0f;
	
	FTimerHandle ReloadTimerHandle;
};