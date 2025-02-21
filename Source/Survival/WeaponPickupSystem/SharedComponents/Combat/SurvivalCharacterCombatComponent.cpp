// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalCharacterCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
#include "Survival/WeaponPickupSystem/Character/GAS/Abilities/CharacterGameplayAbility.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/WeaponData.h"
#include "Survival/WeaponPickupSystem/Libraries/SurvivalAbilitySystemLibrary.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"


USurvivalCharacterCombatComponent::USurvivalCharacterCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USurvivalCharacterCombatComponent::ToggleFootCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		if (const ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(GetOwner()))
		{
			if (bShouldEnable)
			{
				PlayerCharacter->GetFootCollision()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				Debug::Print(PlayerCharacter->GetFootCollision()->GetName() + TEXT(" collision enabled"),FColor::Green);
			}
			else
			{
				PlayerCharacter->GetFootCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Debug::Print(PlayerCharacter->GetFootCollision()->GetName() + TEXT(" collision disabled"),FColor::Red);
			}

			OverlappedActors.Empty(); 
		}
	}
}

float USurvivalCharacterCombatComponent::GetCharacterCurrentEquipWeaponDamageAtLevel(float InLevel) const
{
	return GetCharacterCurrentEquippedWeapon()->GetWeaponDataAsset()->WeaponBaseDamage.GetValueAtLevel(InLevel);
}


void USurvivalCharacterCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	Super::OnHitTargetActor(HitActor);

	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	// TODO: Raycast silah ateşi için overlapped olmasın? Belki de olabilir bak buraya!
	if (GetCharacterCurrentEquippedWeapon()->GetWeaponDataAsset().Get()->WeaponAttributes.WeaponCategory == EWeaponCategory::Ewc_MeleeWeapons) // Projectiler ekle
	{
		OverlappedActors.AddUnique(HitActor);
	}
	
	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		FGameplayTag::RequestGameplayTag(FName("Character.Shared.Event.Hit")),
		EventData
	);

	if (GetCharacterCurrentEquippedWeapon()->GetWeaponDataAsset().Get()->WeaponAttributes.WeaponCategory == EWeaponCategory::Ewc_MeleeWeapons) // Projectile ekle
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			FGameplayTag::RequestGameplayTag(FName("Character.Player.Event.HitPause")),
			FGameplayEventData()
		);
	}
}


void USurvivalCharacterCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	Super::OnWeaponPulledFromTargetActor(InteractedActor);

	// if (!OverlappedActors.IsEmpty())
	// {
	// 	OverlappedActors.Remove(InteractedActor);
	// }
	// OverlappedActors.Empty();
	
}
