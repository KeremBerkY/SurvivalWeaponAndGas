// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/CharacterGameplayAbility.h"
#include "SprintAbility.generated.h"

class UCharacterAbilitySystemComponent;
class UGameplayEffect;


UCLASS()
class SURVIVAL_API USprintAbility : public UCharacterGameplayAbility
{
	GENERATED_BODY()


protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	FActiveGameplayEffectHandle  ApplyGameplayEffect(UCharacterAbilitySystemComponent* AbilitySystem, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 Level);
	UFUNCTION()
	void OnInputRelease(float TimeHeld);
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> SprintEffect;

private:
	FActiveGameplayEffectHandle ActiveSprintEffectHandle;

	float DefaultMovementSpeed;
};
