// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponHeavyAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Libraries/SurvivalAbilitySystemLibrary.h"

UMeleeWeaponHeavyAttack::UMeleeWeaponHeavyAttack()
{
	const auto AbilityTag = FGameplayTag::RequestGameplayTag(FName("Character.Player.Ability.Attack.Heavy"));
	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);

	CurrentHeavyAttackComboCount = 1;
	UsedComboCount = 0;

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMeleeWeaponHeavyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (PlayerCharacterRef)
	{
		ClearComboResetTimer();

		if (USurvivalAbilitySystemLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromCharacterGameplayAbility(), FGameplayTag::RequestGameplayTag(FName("Character.Player.Status.JumpToFinisher"))))
		{
			CurrentHeavyAttackComboCount = AttackMontagesMap.Num();
		}
		UsedComboCount = CurrentHeavyAttackComboCount;
		
		HandleComboAndSetMontage();
		StartAnimMontage();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UMeleeWeaponHeavyAttack::HandleComboAndSetMontage()
{
	if (AttackMontagesMap.Contains(CurrentHeavyAttackComboCount))
	{
		if (UAnimMontage* SelectedMontage = AttackMontagesMap[CurrentHeavyAttackComboCount])
		{
			AbilityMontage = SelectedMontage;
		}
	}

	if (CurrentHeavyAttackComboCount == AttackMontagesMap.Num())
	{
		ResetAttackComboCount();
	}
	else
	{
		if (CurrentHeavyAttackComboCount == AttackMontagesMap.Num() - 1)
		{
			USurvivalAbilitySystemLibrary::AddGameplayTagToActorIfNone(GetPlayerCharacterFromCharacterGameplayAbility(), FGameplayTag::RequestGameplayTag(FName("Character.Player.Status.JumpToFinisher")));
		}
		CurrentHeavyAttackComboCount++;
	}
}

void UMeleeWeaponHeavyAttack::ResetAttackComboCount()
{
	CurrentHeavyAttackComboCount = 1;
	USurvivalAbilitySystemLibrary::RemoveGameplayTagFromActorIfFound(GetPlayerCharacterFromCharacterGameplayAbility(), FGameplayTag::RequestGameplayTag(FName("Character.Player.Status.JumpToFinisher")));
}

void UMeleeWeaponHeavyAttack::SetComboResetTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		ComboCountResetTimerHandle,
		this,
		&UMeleeWeaponHeavyAttack::ResetAttackComboCount,
		0.3f,
		false
	);
}

void UMeleeWeaponHeavyAttack::ClearComboResetTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ComboCountResetTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(ComboCountResetTimerHandle);
	}
}

void UMeleeWeaponHeavyAttack::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnEventReceived(EventTag, Payload);

	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Character.Shared.Event.Hit")))
	{
		if (AActor* LocalTargetActor = Cast<AActor>(Payload.Target))
		{
			if (const USurvivalCharacterCombatComponent* HeroCombatComponent = GetPlayerCharacterFromCharacterGameplayAbility()->GetSurvivalCharacterCombatComponent())
			{
				const float WeaponBaseDamage = HeroCombatComponent->GetCharacterCurrentEquipWeaponDamageAtLevel(GetAbilityLevel());
				const auto SpecHandle = MakeCharacterDamageEffectSpecHandle(
					DamageEffect,
					WeaponBaseDamage,
					FGameplayTag::RequestGameplayTag(FName("Character.SetByCaller.AttackType.Heavy")),
					UsedComboCount
				);
				NativeApplyEffectSpecHandleToTarget(LocalTargetActor, SpecHandle);

				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
					LocalTargetActor,
					FGameplayTag::RequestGameplayTag(FName("Character.Shared.Event.HitReact")),
					Payload
				);
			}
		}
	}
	
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Character.Event.OnComplete")))
	{
		OnCompleted(EventTag, Payload);
	}
}

void UMeleeWeaponHeavyAttack::OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCancelled(EventTag, Payload);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMeleeWeaponHeavyAttack::OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCompleted(EventTag, Payload);
	
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


void UMeleeWeaponHeavyAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	SetComboResetTimer();
}

