// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMontageEventAbility.h"

#include "AbilityTasks/CharacterAbilityTask_PlayMontageAndWaitForEvent.h"
#include "Survival/SurvivalCharacter.h"

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
	Task = UCharacterAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this,
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

void UCharacterMontageEventAbility::BlockAbilities(bool IsEnableBlockAbilities)
{
	FGameplayTagContainer Temp;
	PlayerCharacterRef->GetCharacterAbilitySystemComponent()->ApplyAbilityBlockAndCancelTags(
		Temp, this, IsEnableBlockAbilities, BlockTagList, false, Temp);
}
