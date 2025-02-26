// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnAbility.h"

#include "AbilitySystemComponent.h"
#include "Survival/WeaponPickupSystem/SharedComponents/Combat/EnemyCombatComponent.h"
#include "Survival/WeaponPickupSystem/SharedComponents/Combat/PawnCombatComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/MeleeWeapons/MeleeWeapon.h"

void USpawnAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	if (AbilityActivationPolicy == ESrurvivalAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void USpawnAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (WeaponClassToSpawn)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetAvatarActorFromActorInfo();
		SpawnParams.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());

		if (AMeleeWeapon* SpawnedActor = GetWorld()->SpawnActor<AMeleeWeapon>(WeaponClassToSpawn ,SpawnParams))
		{
			if (ActorInfo && ActorInfo->AvatarActor.IsValid())
			{
				USkeletalMeshComponent* SkeletalMeshComponent = nullptr;
				if (const APawn* Pawn = Cast<APawn>(ActorInfo->AvatarActor.Get()))
				{
					SkeletalMeshComponent = Pawn->FindComponentByClass<USkeletalMeshComponent>();
				}

				const FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
				
				SpawnedActor->AttachToComponent(SkeletalMeshComponent, AttachRules, SocketNameToAttachTo);
				
				if (WeaponTagToRegister.IsValid())
				{
					if (const auto EnemyCombatComponent = GetAvatarActorFromActorInfo()->FindComponentByClass<UEnemyCombatComponent>())
					{
						EnemyCombatComponent->RegisterSpawnedWeapon(WeaponTagToRegister, SpawnedActor, RegisterAsEquippedWeapon);
					}
				}
			}
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void USpawnAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == ESrurvivalAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}
