// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Interfaces/WeaponUIHandler.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/WeaponCategoriesUIHandlers/WeaponUIHandlerBase.h"
#include "RaycastWeaponUIHandler.generated.h"


class ARaycastWeapons;
class ASurvivalCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowRaycastUI);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHideUI);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitializeRaycastCallbacks, AWeaponBase*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateHeatBar, float, Heat, float, MaxHeat);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateHeatBar, AWeaponBase*, Weapon);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API URaycastWeaponUIHandler : public UWeaponUIHandlerBase
{
	GENERATED_BODY()

public:
	URaycastWeaponUIHandler();
	
	FOnShowRaycastUI ShowUI;
	FOnHideUI HideUI;
	FOnInitializeRaycastCallbacks OnInitializeCallbacks;
	FOnUpdateHeatBar OnUpdateHeatBar;
	
	void UpdateHeatBar(float Heat, float MaxHeat);
protected:
	virtual void BeginPlay() override;

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
	ARaycastWeapons* OwnedWeapon;
	
	
};
