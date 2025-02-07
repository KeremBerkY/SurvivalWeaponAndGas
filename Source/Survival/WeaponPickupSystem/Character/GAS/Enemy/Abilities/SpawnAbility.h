// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Enemy/SurvivalEnemyGameplayAbility.h"
#include "SpawnAbility.generated.h"

class AMeleeWeapon;

UENUM(BlueprintType)
enum class ESrurvivalAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

/**
 * 
 */
UCLASS()
class SURVIVAL_API USpawnAbility : public USurvivalEnemyGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "WarriorAbility")
	ESrurvivalAbilityActivationPolicy AbilityActivationPolicy = ESrurvivalAbilityActivationPolicy::OnTriggered;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AMeleeWeapon> WeaponClassToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName SocketNameToAttachTo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FGameplayTag WeaponTagToRegister;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	bool RegisterAsEquippedWeapon;
};
