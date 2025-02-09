// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastAttackAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"

URaycastAttackAbility::URaycastAttackAbility()
{
	const auto AbilityTag = FGameplayTag::RequestGameplayTag(FName("Character.Player.Ability.Attack.Raycast"));
    AbilityTags.AddTag(AbilityTag);
    ActivationOwnedTags.AddTag(AbilityTag);
}

void URaycastAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (PlayerCharacterRef && PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon())
	{
		if (const ARaycastWeapons* RaycastWeaponsRef = Cast<ARaycastWeapons>(PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon()))
		{
			// Aşağıdaki AbilityMontage ve MontageRate'yi kaldırman lazım. Çünkü bütün yapıyı değiştirmek zorunda kalıcaz öbür türlü.
			PlayerCharacterRef->GetCharacterWeaponComponent()->GetCurrentWeapon()->Attack();

			/*          AŞAĞIDAKİNİ FIREMODE İÇERİSİNE KOYACAKSIN AUTOMATİC BURST GİBİ LOOP'U BU ŞEKİLDE YÖNETECEKSİN        */
			// if (OwnerWeapon->GetOwningCharacter()->GetAbilitySystemComponent()->TryActivateAbilityByClass())
			// {
			// 	 TODO: Silahın RangedWeapon kısmına Fire Ability koyarsak. Bunu AutomaticFire üzerinden sürekli ateş edebiliriz. Loopa alıp çağırabiliriz.
			//		Ability her çağrıldığında tekrar Attack çağrılacak. Bunun önüne geçmek için bool koy belkide tag koyabilirsin. üzerinde düşün!
			//		Tag koyarsak automatic atış modunda olduğumuz bilinir ve modlara göre bir ayar yapabiliriz.
			// }
			
			AbilityMontage = RaycastWeaponsRef->GetRaycastWeaponDataAsset()->WeaponAnimMontages.FireMontage;
			MontageRate = RaycastWeaponsRef->GetRaycastWeaponDataAsset()->FireRate;
			
			StartAnimMontage();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character has no Weapon"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	// UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	// WaitInputReleaseTask->OnRelease.AddDynamic(this, &URaycastAttackAbility::OnInputRelease);
	// WaitInputReleaseTask->Activate();
}

void URaycastAttackAbility::OnInputRelease(float TimeHeld)
{
	// TODO: AutomaticShot da ateş etmeye devam ediyor. Bunu Kaldırmamız lazım! (Shot Modelarına Tag ekleyelim. Burada da Tagler check edilsin.
	// Örneğin AutomaticShot aktifse EndAbility çalışmasın. Burada Tag kontrolü yapıcaz. 
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void URaycastAttackAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData Payload)
{
	Super::OnEventReceived(EventTag, Payload);

	if (EventTag == FGameplayTag::RequestGameplayTag("Character.Shared.Event.RaycastHit"))
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

