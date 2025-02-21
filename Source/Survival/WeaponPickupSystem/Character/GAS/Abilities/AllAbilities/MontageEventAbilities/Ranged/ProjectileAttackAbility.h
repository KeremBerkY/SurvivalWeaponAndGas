// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/CharacterMontageEventAbility.h"
#include "ProjectileAttackAbility.generated.h"

class AProjectileWeapons;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UProjectileAttackAbility : public UCharacterMontageEventAbility
{
	GENERATED_BODY()

protected:
	UProjectileAttackAbility();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// UFUNCTION()
	// virtual void StopFire(FGameplayEventData GameplayEventData);
	// virtual void HandleApplyDamage(const FGameplayEventData& GameplayEventData);
	

	TWeakObjectPtr<AProjectileWeapons> ProjectileWeaponPtr;
	
	UPROPERTY()
	bool bIsStopFire;
};
