// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RaycastEffectManagerComponent.generated.h"


class ARaycastWeapons;
class UProjectileWeaponData;
class URaycastWeaponData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API URaycastEffectManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URaycastEffectManagerComponent();

	void ApplyRecoilEffect() const;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void InitializeEffects();
	UFUNCTION()
	void PlayWeaponEffects();
	UFUNCTION()
	void PlayImpactEffects(const FHitResult& HitResult);

	void WeaponEffect() const;
	void ResetRecoil();
	void ResetWeaponPosition(FVector OriginalLocation) const;
	void ImpactEffect(const FHitResult& HitResult) const;
	void ApplyDecal(const FHitResult& HitResult) const;

private:
	ARaycastWeapons* Weapon;
	URaycastWeaponData* RaycastWeaponData;

	bool bCanApplyRecoil;
	FTimerHandle RecoilCooldownTimerHandle;
};
