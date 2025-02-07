// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastWeaponSwitchAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/WeaponInventory.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/LockonComponent.h"

void URaycastWeaponSwitchAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Raycast Switch Activated"));
	
	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get()))
	{
		const UCharacterWeaponComponent* CharacterWeaponComponent = PlayerCharacter->GetCharacterWeaponComponent();
		if (CharacterWeaponComponent && CharacterWeaponComponent->CanSwitchWeapon() && !PlayerCharacter->GetLockonComponent()->IsLocked())
		{
			PlayerCharacter->GetWeaponInventory()->SwapToBackWeapon(CharacterWeaponComponent->GetCurrentWeapon(), PlayerCharacter, EWeaponCategory::Ewc_RaycastWeapons);
			PlayerCharacter->GetCharacterWeaponComponent()->UpdateLastSwitchTime(); // SORU: CharacterWeaponComponent ile UpdateLastSwitchTime() çağıramadım neden?
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URaycastWeaponSwitchAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UE_LOG(LogTemp, Warning, TEXT("Raycast Switch END"));

}

