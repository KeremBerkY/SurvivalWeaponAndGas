// Fill out your copyright notice in the Description page of Project Settings.


#include "FireAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"


void UFireAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FireHandleController(ActorInfo,true);

	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &UFireAbility::OnInputRelease);
	WaitInputReleaseTask->Activate();
}

void UFireAbility::OnInputRelease(float TimeHeld)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UFireAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	FireHandleController(ActorInfo,false);
	
}

void UFireAbility::FireHandleController(const FGameplayAbilityActorInfo* ActorInfo, bool bPressed)
{
	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get()))
	{
		UCharacterWeaponComponent* CharacterWeaponComponent = PlayerCharacter->GetCharacterWeaponComponent();
		if (CharacterWeaponComponent && CharacterWeaponComponent->GetCurrentWeapon() && CharacterWeaponComponent->GetCurrentWeapon()->CanFire())
		{
			CharacterWeaponComponent->GetCurrentWeapon()->FireHandle(bPressed);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("You have no weapon!"));
		}
	}
}
