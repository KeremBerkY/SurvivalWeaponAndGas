// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAttackAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/ProjectileWeaponData/ProjectileWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/ProjectileWeapons.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"


UProjectileAttackAbility::UProjectileAttackAbility()
{
}

void UProjectileAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const auto WeaponRef = PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon();
	if (PlayerCharacterRef && WeaponRef)
	{
		ProjectileWeaponPtr = MakeWeakObjectPtr(Cast<AProjectileWeapons>(PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon()));
		const auto ProjectileWeaponDataAsset = ProjectileWeaponPtr->GetProjectileWeaponDataAsset();
		
		check(ProjectileWeaponDataAsset);
	
		AbilityMontage = ProjectileWeaponDataAsset->WeaponAnimMontages;
		MontageRate = 1 + (ProjectileWeaponDataAsset->FireRate);
		// bIsStopFire = false;
		StartAnimMontage();
		ProjectileWeaponPtr->Attack();
		
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	
}

void UProjectileAttackAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnEventReceived(EventTag, Payload);
	
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Character.Shared.Event.Hit")))
	{
		HandleApplyDamage(Payload);
	}
}

void UProjectileAttackAbility::HandleApplyDamage(const FGameplayEventData& GameplayEventData)
{
	if (AActor* LocalTargetActor = Cast<AActor>(GameplayEventData.Target))
	{
		if (USurvivalCharacterCombatComponent* SurvivalCharacterCombatComponent = GetPlayerCharacterFromCharacterGameplayAbility()->GetSurvivalCharacterCombatComponent())
		{
			const float WeaponBaseDamage = SurvivalCharacterCombatComponent->GetCharacterCurrentEquipWeaponDamageAtLevel(GetAbilityLevel());
			const auto SpecHandle = MakeCharacterDamageEffectSpecHandle(
				DamageEffect,
				WeaponBaseDamage,
				FGameplayTag::RequestGameplayTag(FName("Character.SetByCaller.AttackType.Ray"))
			);
			NativeApplyEffectSpecHandleToTarget(LocalTargetActor, SpecHandle);

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				LocalTargetActor,
				FGameplayTag::RequestGameplayTag(FName("Character.Shared.Event.HitReact")),
				GameplayEventData
			);

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			LocalTargetActor,
			FGameplayTag::RequestGameplayTag(FName("Character.Shared.Event.ThrowBack")),
			GameplayEventData
		);
		}
	}
}

void UProjectileAttackAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCancelled(EventTag, Payload);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UProjectileAttackAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCompleted(EventTag, Payload);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UProjectileAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

