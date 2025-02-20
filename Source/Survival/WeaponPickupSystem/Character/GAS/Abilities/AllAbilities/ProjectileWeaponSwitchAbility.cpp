// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeaponSwitchAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/WeaponInventory.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Character/Components/LockonComponent.h"

void UProjectileWeaponSwitchAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get()))
	{
		UCharacterWeaponComponent* CharacterWeaponComponent = PlayerCharacter->GetCharacterWeaponComponent();

		if (const auto WeaponInventory = PlayerCharacter->GetWeaponInventory())
		{
			
			if (WeaponInventory->HasWeaponInCategory(EWeaponCategory::Ewc_ProjectileWeapons))
			{
				if (CharacterWeaponComponent && CharacterWeaponComponent->CanSwitchWeapon() && !PlayerCharacter->GetLockonComponent()->IsLocked())
				{
					WeaponInventory->SwapToBackWeapon(
						CharacterWeaponComponent->GetCurrentWeapon(),
						PlayerCharacter,
						EWeaponCategory::Ewc_ProjectileWeapons
					);
					CharacterWeaponComponent->UpdateLastSwitchTime();
				}
			}
			else
			{
				if (CharacterWeaponComponent->GetCurrentWeapon()->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory == EWeaponCategory::Ewc_ProjectileWeapons)
				{
					if (CharacterWeaponComponent && CharacterWeaponComponent->CanSwitchWeapon() && !PlayerCharacter->GetLockonComponent()->IsLocked())
					{
						WeaponInventory->SwapToBackWeapon(
							CharacterWeaponComponent->GetCurrentWeapon(),
							PlayerCharacter,
							EWeaponCategory::Ewc_ProjectileWeapons
						);
						CharacterWeaponComponent->UpdateLastSwitchTime();
					}
				}
				else
				{
					WeaponInventory->SwapToBackWeapon(
						CharacterWeaponComponent->GetCurrentWeapon(),
						PlayerCharacter,
						CharacterWeaponComponent->GetCurrentWeapon()->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory
					);
				}
			}

	
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

}

void UProjectileWeaponSwitchAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}
		
// if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get()))
// {
// 	const UCharacterWeaponComponent* CharacterWeaponComponent = PlayerCharacter->GetCharacterWeaponComponent();
// 	
// 	if (CharacterWeaponComponent && CharacterWeaponComponent->CanSwitchWeapon() && !PlayerCharacter->GetLockonComponent()->IsLocked())
// 	{
// 		PlayerCharacter->GetWeaponInventory()->SwapToBackWeapon(CharacterWeaponComponent->GetCurrentWeapon(), PlayerCharacter, EWeaponCategory::Ewc_ProjectileWeapons);
// 		PlayerCharacter->GetCharacterWeaponComponent()->UpdateLastSwitchTime();
// 	}
// }