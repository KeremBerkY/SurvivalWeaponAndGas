// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/WeaponCategoriesUIHandlers/WeaponUIHandlerBase.h"
#include "MeleeWeaponUIHandler.generated.h"

class AMeleeWeapon;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMeleeShowUI);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMeleeHideUI);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitializeMeleeCallbacks, AWeaponBase*, Weapon);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UMeleeWeaponUIHandler : public UWeaponUIHandlerBase
{
	GENERATED_BODY()

public:
	UMeleeWeaponUIHandler();

	FOnMeleeShowUI ShowUI;
	FOnMeleeShowUI HideUI;
	FOnInitializeMeleeCallbacks OnInitializeMeleeCallbacks;

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
	AMeleeWeapon* OwnedWeapon;
};
