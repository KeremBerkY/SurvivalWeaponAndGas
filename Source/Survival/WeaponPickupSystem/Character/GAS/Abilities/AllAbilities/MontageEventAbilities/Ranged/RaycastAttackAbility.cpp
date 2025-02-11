// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastAttackAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/HeatComponent/HeatComponent.h"

URaycastAttackAbility::URaycastAttackAbility()
{
	const auto AbilityTag = FGameplayTag::RequestGameplayTag(FName("Character.Player.Ability.Attack.Raycast"));
    AbilityTags.AddTag(AbilityTag);
    ActivationOwnedTags.AddTag(AbilityTag);
}

void URaycastAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const auto WeaponRef = PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon();
	if (PlayerCharacterRef && WeaponRef)
	{
		const ARaycastWeapons* RaycastWeaponsRef = Cast<ARaycastWeapons>(PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon());
		const auto RaycastWeaponData = RaycastWeaponsRef->GetRaycastWeaponDataAsset();
		// !RaycastWeaponsRef->CanFire()
		if (!RaycastWeaponsRef || RaycastWeaponsRef->GetCurrentAmmo() <= 0 ||
			RaycastWeaponsRef->GetHeatComponent()->GetCurrentHeat() >= RaycastWeaponData->FiringHeatSettings.MaxHeatCapacity ||
			RaycastWeaponsRef->GetHeatComponent()->IsOverHeated())
		{
			WeaponRef->EndAttack();
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			return;
		};

	
		AbilityMontage = RaycastWeaponsRef->GetRaycastWeaponDataAsset()->WeaponAnimMontages.FireMontage;
		MontageRate = 1 + (RaycastWeaponsRef->GetRaycastWeaponDataAsset()->FireRate);

		StartAnimMontage();
		PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon()->Attack();
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character has no Weapon"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &URaycastAttackAbility::OnInputRelease);
	WaitInputReleaseTask->Activate();
}

void URaycastAttackAbility::OnInputRelease(float TimeHeld)
{
	// TODO: AutomaticShot da ateş etmeye devam ediyor. Bunu Kaldırmamız lazım! (Shot Modelarına Tag ekleyelim. Burada da Tagler check edilsin.
	// Örneğin AutomaticShot aktifse EndAbility çalışmasın. Burada Tag kontrolü yapıcaz.
	
	if (PlayerCharacterRef)
	{
		const auto CurrentWeapon = PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon();
		if (CurrentWeapon && !CurrentWeapon->GetAttackCooldownActive())
		{
			CurrentWeapon->EndAttack();
			Debug::Print("EndAbility Called!", FColor::Orange);
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URaycastAttackAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnEventReceived(EventTag, Payload);

	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Character.Shared.Event.Hit")))
	{
		HandleApplyDamage(Payload);
	}

	// EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URaycastAttackAbility::HandleApplyDamage(const FGameplayEventData& Payload)
{
	Debug::Print("Handle Apply Damage Called!", FColor::Purple);
}

void URaycastAttackAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCancelled(EventTag, Payload);
	
	// EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URaycastAttackAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnCompleted(EventTag, Payload);

	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void URaycastAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// if (PlayerCharacterRef)
	// {
	// 	const auto CurrentWeapon = PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon();
	// 	if (CurrentWeapon && !CurrentWeapon->GetAttackCooldownActive())
	// 	{
	// 		CurrentWeapon->EndAttack();
	// 		Debug::Print("EndAbility Called!", FColor::Orange);
	// 	}
	// }
}

