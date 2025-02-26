// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.h"
#include "EnemyCombatComponent.generated.h"


struct FGameplayTag;
class AMeleeWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UEnemyCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnemyCombatComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Survival|EnemyCombat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AMeleeWeapon* InWeaponToRegister, bool bRegisterEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AMeleeWeapon* GetEnemyCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;
	
	UPROPERTY(BlueprintReadWrite, Category = "Survival|EnemyCombar")
	FGameplayTag CurrentEquippedWeaponTag;
	
	UFUNCTION(BlueprintCallable, Category = "Survival|EnemyCombat")
	AMeleeWeapon* GetEnemyCurrentEquippedWeapon() const;

private:
	TMap<FGameplayTag, AMeleeWeapon*> EnemyCarriedWeaponMap;
};
