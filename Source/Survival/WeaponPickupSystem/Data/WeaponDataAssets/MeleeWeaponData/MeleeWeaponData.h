// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/WeaponData.h"
#include "MeleeWeaponData.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UMeleeWeaponData : public UWeaponData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Animation")
	UAnimMontage* AbilityMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Animation")
	float MontageRate;
};
