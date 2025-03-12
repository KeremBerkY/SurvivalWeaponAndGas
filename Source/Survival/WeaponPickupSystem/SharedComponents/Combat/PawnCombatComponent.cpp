// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCombatComponent.h"

#include "Components/BoxComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/WeaponData.h"


UPawnCombatComponent::UPawnCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

AWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (const ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(GetOwner()))
	{
		if (AWeaponBase* CurrentWeapon = PlayerCharacter->GetCharacterWeaponComponent()->GetCurrentWeapon())
		{
			return CurrentWeapon;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Current Weapon is not a melee wapon"));
	return nullptr;
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	// TODO: Bu parent sınıfta değil Karakter sınıfında olmalı enemy ve karakter override edecek ve kendi işlevlerini yazacak!
	
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		const AWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();

		check(WeaponToToggle);
		
		if (bShouldEnable)
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			if (WeaponToToggle->GetWeaponHandleCollisionBox())
			{
				WeaponToToggle->GetWeaponHandleCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}
		}
		else
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			if (WeaponToToggle->GetWeaponHandleCollisionBox())
			{
				WeaponToToggle->GetWeaponHandleCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				
			}
			
			OverlappedActors.Empty(); 
		}
	}
}

void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}

