// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterGameplayAbility.h"

#include "EnhancedInputComponent.h"
#include "Survival/SurvivalCharacter.h"

UCharacterGameplayAbility::UCharacterGameplayAbility()
{
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
	// ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));
	// ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Fear")));

	// CooldownTag = FGameplayTag::RequestGameplayTag(FName("Data.Cooldown"));
}


const FGameplayTagContainer* UCharacterGameplayAbility::GetCooldownTags() const
{
	if (!bHasCooldown)
	{
		return Super::GetCooldownTags();
	}
	
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	MutableTags->Reset();

	const auto ParentTags = Super::GetCooldownTags();
	if (ParentTags)
	{
		MutableTags->AppendTags(*ParentTags);
	}

	MutableTags->AppendTags(CooldownTags);

	return MutableTags;
}

void UCharacterGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);

	if (!bHasCooldown)
	{
		return;
	}

	const auto GECooldown = GetCooldownGameplayEffect();
	if (IsValid(GECooldown) && GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		const auto SpecHandle = MakeOutgoingGameplayEffectSpec(GECooldown->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);

		FinalCooldownDuration = CooldownDuration.GetValueAtLevel(1);

		SpecHandle.Data.Get()->SetSetByCallerMagnitude(CooldownTag, FinalCooldownDuration);

		ActiveCooldownEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}

void UCharacterGameplayAbility::CancelCooldown() const
{
	if (bHasCooldown)
	{
		if (const auto AbilitySystemComponent= GetAbilitySystemComponentFromActorInfo())
		{
			if (ActiveCooldownEffectHandle.IsValid())
			{
				AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveCooldownEffectHandle);
			}
		}
	}
}

void UCharacterGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UCharacterGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
}

void UCharacterGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	OnAbilityEnded.Broadcast(this, bWasCancelled);
}

ASurvivalCharacter* UCharacterGameplayAbility::GetPlayerCharacterFromCharacterGameplayAbility()
{
	if (!CachedSurvivalCharacter.IsValid())
	{
		CachedSurvivalCharacter = Cast<ASurvivalCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedSurvivalCharacter.IsValid() ? CachedSurvivalCharacter.Get() : nullptr;
}
