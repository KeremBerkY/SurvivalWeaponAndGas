// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponLightAttack.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
#include "Survival/WeaponPickupSystem/Libraries/SurvivalAbilitySystemLibrary.h"

UMeleeWeaponLightAttack::UMeleeWeaponLightAttack()
{
	const auto AbilityTag = FGameplayTag::RequestGameplayTag(FName("Character.Player.Ability.Attack.Light"));
	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);

	CurrentLightAttackComboCount = 1;
	UsedComboCount = 0;

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMeleeWeaponLightAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (PlayerCharacterRef)
	{
		ClearComboResetTimer();

		UsedComboCount = CurrentLightAttackComboCount;
		
		HandleComboAndSetMontage();
		StartAnimMontage();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UMeleeWeaponLightAttack::HandleComboAndSetMontage()
{
	if (AttackMontagesMap.Contains(CurrentLightAttackComboCount))
	{
		if (UAnimMontage* SelectedMontage = AttackMontagesMap[CurrentLightAttackComboCount])
		{
			AbilityMontage = SelectedMontage;
		}
	}

	if (CurrentLightAttackComboCount == AttackMontagesMap.Num())
	{
		ResetAttackComboCount();
	}
	else
	{
		if (CurrentLightAttackComboCount == AttackMontagesMap.Num() - 1)
		{
			USurvivalAbilitySystemLibrary::AddGameplayTagToActorIfNone(GetPlayerCharacterFromCharacterGameplayAbility(), FGameplayTag::RequestGameplayTag(FName("Character.Player.Status.JumpToFinisher")));
		}
		CurrentLightAttackComboCount++;
	}
}

void UMeleeWeaponLightAttack::ResetAttackComboCount()
{
	CurrentLightAttackComboCount = 1;

	USurvivalAbilitySystemLibrary::RemoveGameplayTagFromActorIfFound(GetPlayerCharacterFromCharacterGameplayAbility(), FGameplayTag::RequestGameplayTag(FName("Character.Player.Status.JumpToFinisher")));
}

void UMeleeWeaponLightAttack::SetComboResetTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		ComboCountResetTimerHandle,
		this,
		&UMeleeWeaponLightAttack::ResetAttackComboCount,
		0.3f,
		false
	);

}

void UMeleeWeaponLightAttack::ClearComboResetTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ComboCountResetTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(ComboCountResetTimerHandle);
	}
}

void UMeleeWeaponLightAttack::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnEventReceived(EventTag, Payload);

	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Character.Shared.Event.Hit")))
	{
		HandleApplyDamage(Payload);
	}
	
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Character.Event.OnComplete")))
	{
		OnCompleted(EventTag, Payload);
	}
}

void UMeleeWeaponLightAttack::HandleApplyDamage(const FGameplayEventData& GameplayEventData)
{
	if (AActor* LocalTargetActor = Cast<AActor>(GameplayEventData.Target))
	{
		if (USurvivalCharacterCombatComponent* SurvivalCharacterCombatComponent = GetPlayerCharacterFromCharacterGameplayAbility()->GetSurvivalCharacterCombatComponent())
		{
			const float WeaponBaseDamage = SurvivalCharacterCombatComponent->GetCharacterCurrentEquipWeaponDamageAtLevel(GetAbilityLevel());
			const auto SpecHandle = MakeCharacterDamageEffectSpecHandle(
				DamageEffect,
				WeaponBaseDamage,
				FGameplayTag::RequestGameplayTag(FName("Character.SetByCaller.AttackType.Light")),
				UsedComboCount
			);
			NativeApplyEffectSpecHandleToTarget(LocalTargetActor, SpecHandle);
		}
	}
}

void UMeleeWeaponLightAttack::OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCancelled(EventTag, Payload);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMeleeWeaponLightAttack::OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCompleted(EventTag, Payload);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMeleeWeaponLightAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	SetComboResetTimer();
}


