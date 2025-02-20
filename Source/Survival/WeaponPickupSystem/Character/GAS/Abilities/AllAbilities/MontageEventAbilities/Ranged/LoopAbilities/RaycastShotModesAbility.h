// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/CharacterMontageEventAbility.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/AllAbilities/MontageEventAbilities/Ranged/RaycastAttackAbility.h"
#include "RaycastShotModesAbility.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API URaycastShotModesAbility : public URaycastAttackAbility
{
	GENERATED_BODY()

protected:
	URaycastShotModesAbility();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
// 	UFUNCTION()
// 	void OnInputRelease(float TimeHeld);
	virtual void OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
//
	// virtual void HandleApplyDamage(const FGameplayEventData& GameplayEventData) override;
//
// private:
// 	
// 	TWeakObjectPtr<ARaycastWeapons> RaycastWeaponPtr;
};
