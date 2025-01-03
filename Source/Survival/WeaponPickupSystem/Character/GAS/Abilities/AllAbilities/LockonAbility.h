// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/CharacterGameplayAbility.h"
#include "LockonAbility.generated.h"

class ULockonComponent;
class ASurvivalCharacter;
/**
 * 
 */
UCLASS()
class SURVIVAL_API ULockonAbility : public UCharacterGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	void OnInputRelease(float TimeHeld);
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:

	TWeakObjectPtr<ASurvivalCharacter> PlayerCharacterPtr;
	TWeakObjectPtr<ULockonComponent> LockonComponentPtr;
	TWeakObjectPtr<ULockonComponent> FocusCrosshairPtr;
	
	bool bIsLocked;
	bool bLockEnded;
};
