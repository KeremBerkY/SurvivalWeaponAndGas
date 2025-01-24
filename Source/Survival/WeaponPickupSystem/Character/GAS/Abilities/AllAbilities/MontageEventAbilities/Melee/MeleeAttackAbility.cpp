// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttackAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/WeaponData.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/MeleeWeaponData/MeleeWeaponData.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/AbilityTasks/CharacterAbilityTask_PlayMontageAndWaitForEvent.h"


UMeleeAttackAbility::UMeleeAttackAbility()
{
	const auto AbilityTag = FGameplayTag::RequestGameplayTag("Weapon.State.Melee.Attack");

	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
}

void UMeleeAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
										  const FGameplayAbilityActorInfo* ActorInfo,
										  const FGameplayAbilityActivationInfo ActivationInfo,
										  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (PlayerCharacterRef &&
		PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon()->GetWeaponDataAsset().Get()->WeaponAttributes.WeaponCategory == EWeaponCategory::Ewc_MeleeWeapons)
	{
		if (const auto MeleeWeaponData = Cast<UMeleeWeaponData>(PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon()->GetWeaponDataAsset()))
		{
			AbilityMontage = MeleeWeaponData->AbilityMontage;
			MontageRate = MeleeWeaponData->MontageRate;
		}

		
		UAbilityTask_WaitGameplayEvent* WaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this, FGameplayTag::RequestGameplayTag("Weapon.State.Melee.EndAttack"));
		WaitTask->EventReceived.AddDynamic(this, &UMeleeAttackAbility::StopAttack);
		WaitTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	
}

void UMeleeAttackAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnEventReceived(EventTag, Payload);

	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Animation.Weapon.Attack")))
	{
		Attack();
	}
}

void UMeleeAttackAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCompleted(EventTag, Payload);

	if (PlayerCharacterRef && PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon())
	{
		const auto MeleeWeaponData = Cast<UMeleeWeaponData>(PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon()->GetWeaponDataAsset());
		MontageRate = MeleeWeaponData->MontageRate;
	}
}

void UMeleeAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (PlayerCharacterRef && PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon())
	{
		// Melee saldırısı bittiğini bildiren bir olay tetikleyebilirsiniz
		// PlayerCharacterRef->GetCharacterWeaponComponent()->OnMeleeAttackFinished.Broadcast();
	}
}

void UMeleeAttackAbility::StopAttack(FGameplayEventData GameplayEventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMeleeAttackAbility::AttackSeq()
{
	if (PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon())
	{
		StartAnimMontage();
		// TODO: PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon().OnMeleeAttackStarted.Broadcast();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UMeleeAttackAbility::Attack()
{
	// Kılıç savurma mantığını buraya ekleyin
	// Örneğin, yakın dövüş saldırısı için bir raycast yapabilirsiniz:
	// Örnek yapı aşağıda;
	FVector Start = PlayerCharacterRef->GetActorLocation();
	FVector ForwardVector = PlayerCharacterRef->GetActorForwardVector();
	FVector End = Start + (ForwardVector * 200.0f); // 200 birimlik bir ray

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerCharacterRef);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, Params))
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			// Düşmana hasar ver
			// UGameplayStatics::ApplyDamage(HitActor, 50.0f, PlayerCharacterRef->GetController(), this, nullptr);
		}
	}
}
