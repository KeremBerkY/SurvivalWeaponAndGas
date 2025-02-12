// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMontageEventAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilityTasks/CharacterAbilityTask_PlayMontageAndWaitForEvent.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Libraries/SurvivalAbilitySystemLibrary.h"

void UCharacterMontageEventAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	PlayerCharacterRef = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor);
}

void UCharacterMontageEventAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UCharacterMontageEventAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload)
{
}

void UCharacterMontageEventAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
}

void UCharacterMontageEventAbility::StartAnimMontage()
{
	Task = UCharacterAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
		this,
		NAME_None,
		AbilityMontage,
		FGameplayTagContainer(),
		MontageRate,
		NAME_None,
		false,
		1
	);

	Task->OnCompleted.AddDynamic(this, &ThisClass::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &ThisClass::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &ThisClass::OnCancelled);
	Task->EventReceived.AddDynamic(this, &ThisClass::OnEventReceived);
	
	Task->ReadyForActivation();	
}

void UCharacterMontageEventAbility::StopAnimMontage()
{
	if (IsValid(Task))
	{
		Task->EndTask();
		PlayerCharacterRef->GetMesh()->GetAnimInstance()->Montage_Stop(0.25, AbilityMontage);
		Task->OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
	}
}

FGameplayEffectSpecHandle UCharacterMontageEventAbility::MakeCharacterDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage,
															FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetCharacterAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetCharacterAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Character.Shared.SetByCaller.BaseDamage")),
		InWeaponBaseDamage
	);

	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(
			InCurrentAttackTypeTag,
			InUsedComboCount
		);
	}
	
	return EffectSpecHandle;
}

FActiveGameplayEffectHandle UCharacterMontageEventAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	check(TargetASC && InSpecHandle.IsValid());

	return GetCharacterAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*InSpecHandle.Data,
		TargetASC
	);
}

void UCharacterMontageEventAbility::BlockAbilities(bool IsEnableBlockAbilities)
{
	FGameplayTagContainer Temp;
	PlayerCharacterRef->GetCharacterAbilitySystemComponent()->ApplyAbilityBlockAndCancelTags(
		Temp, this, IsEnableBlockAbilities, BlockTagList, false, Temp);
}
