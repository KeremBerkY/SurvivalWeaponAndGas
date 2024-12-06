// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"


bool USprintAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void USprintAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// ApplyGameplayEffect();
	
	if (UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this))
	{
		WaitInputReleaseTask->OnRelease.AddDynamic(this, &USprintAbility::OnInputRelease);
		WaitInputReleaseTask->Activate();
	}
}

// void USprintAbility::ApplyGameplayEffect()
// {
// 	if (SprintEffect)
// 	{
// 		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(SprintEffect, 1);
// 		if (EffectSpecHandle.IsValid())
// 		{
// 			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
// 		}
// 	}
// }

void USprintAbility::OnInputRelease(float TimeHeld)
{
	// Tuş bırakıldığında ability sona erdir
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USprintAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
