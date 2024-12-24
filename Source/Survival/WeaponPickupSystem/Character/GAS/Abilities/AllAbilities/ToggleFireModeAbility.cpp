// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleFireModeAbility.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RangedWeapon.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/FireModesComponent/FireModeBaseComponent.h"

void UToggleFireModeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (ARaycastWeapons* CurrentWeapon = Cast<ARaycastWeapons>(PlayerCharacter->GetCharacterWeaponComponent()->GetCurrentWeapon()))
		{
			const auto FireModeComponents = CurrentWeapon->GetFireModeComponents();
			
			if (FireModeComponents.Num() <= 1)
			{
				UE_LOG(LogTemp, Warning, TEXT("Only one or no fire mode available."));
				return;
			}

			const int32 CurrentFireModeIndex = (CurrentWeapon->GetCurrentFireModeIndex() + 1) % FireModeComponents.Num();
			CurrentWeapon->SetCurrentFireModeIndex(CurrentFireModeIndex);
			const auto CurrentFireModeComponent = FireModeComponents[CurrentFireModeIndex];
			CurrentWeapon->SetCurrentFireModeComponent(CurrentFireModeComponent);	

			if (CurrentFireModeComponent)
			{
				//  TODO: Ateşleme modunun özel bir efekt veya UI koyabilirsin! örn: CurrentFireModeComponent->OnModeActivated();
				const FString ModeName = CurrentFireModeComponent->GetClass()->GetName();
				UE_LOG(LogTemp, Log, TEXT("Switched to fire mode: %s"), *ModeName);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to switch fire mode. CurrentFireModeComponent is nullptr."));
			}
			
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("There is no CurrentWeapon."));
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UToggleFireModeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
