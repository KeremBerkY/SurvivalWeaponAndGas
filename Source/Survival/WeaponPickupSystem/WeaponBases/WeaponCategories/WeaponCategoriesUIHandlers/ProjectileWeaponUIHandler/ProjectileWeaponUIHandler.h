// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/WeaponCategoriesUIHandlers/WeaponUIHandlerBase.h"
#include "ProjectileWeaponUIHandler.generated.h"


class AProjectileWeapons;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileShowUI);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileHideUI);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitializeProjectileCallbacks, AWeaponBase*, Weapon);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UProjectileWeaponUIHandler : public UWeaponUIHandlerBase
{
	GENERATED_BODY()

public:
	UProjectileWeaponUIHandler();

	FOnProjectileShowUI ShowUI;
	FOnProjectileHideUI HideUI;
	FOnInitializeProjectileCallbacks OnInitializeCallbacks;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void InitializeCallbacks(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter) override;
	UFUNCTION()
	virtual void UpdateUI(AWeaponBase* Weapon) override;
	UFUNCTION()
	virtual void SetupUI(AWeaponBase* Weapon) override;
	UFUNCTION()
	virtual void RemoveUI(AWeaponBase* Weapon) override;

private:
	UPROPERTY()
	AProjectileWeapons* OwnedWeapon;
};
