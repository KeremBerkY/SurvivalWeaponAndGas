// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/CharacterGameplayAbility.h"
#include "AimingAbility.generated.h"

class ULockonComponent;
class UCharacterCameraComponent;
class ASurvivalCharacter;
class UCharacterAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class SURVIVAL_API UAimingAbility : public UCharacterGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	void OnInputRelease(float TimeHeld);
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	TWeakObjectPtr<ASurvivalCharacter> PlayerCharacterPtr;
	TWeakObjectPtr<UCharacterCameraComponent> CameraComponentPtr;
	TWeakObjectPtr<ULockonComponent> LockonComponentPtr;
};
