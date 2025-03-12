// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_MeleeAttack_Base.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
#include "Survival/WeaponPickupSystem/Character/GAS/CharacterAbilitySystemComponent.h"

UEnemy_MeleeAttack_Base::UEnemy_MeleeAttack_Base()
{
	const auto AbilityTag = FGameplayTag::RequestGameplayTag(FName("Character.Enemy.Ability.Melee"));
	AbilityTags.AddTag(AbilityTag);
	
	const auto BlockTag = FGameplayTag::RequestGameplayTag(FName("Character.Enemy.Ability"));
	BlockAbilitiesWithTag.AddTag(BlockTag);

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEnemy_MeleeAttack_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AbilityMontage = MontageToPlay;
	StartAnimMontage();
	
}

void UEnemy_MeleeAttack_Base::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnEventReceived(EventTag, Payload);

	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Character.Shared.Event.Hit")))
	{
		HandleApplyDamage(Payload);
	}
}

void UEnemy_MeleeAttack_Base::HandleApplyDamage(const FGameplayEventData& GameplayEventData)
{
	const FGameplayEffectSpecHandle SpecHandle = MakeEnemyDamageEffectSpecHandle(DamageEffect, InDamageScalableFloat);
	if (SpecHandle.IsValid())
	{
		if (const auto TargetActor = Cast<AActor>(GameplayEventData.Target))
		{
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
			{
				GetCharacterAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetASC);
			}
		}
	}
}

void UEnemy_MeleeAttack_Base::OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCancelled(EventTag, Payload);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UEnemy_MeleeAttack_Base::OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCompleted(EventTag, Payload);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UEnemy_MeleeAttack_Base::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

