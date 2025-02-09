// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalCharacterCombatComponent.h"

#include "Components/BoxComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/SurvivalDebugHelper.h"
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
				Debug::Print(PlayerCharacter->GetFootCollision()->GetName() + TEXT(" collision enabled"),FColor::Red);
			}
		}
	}
}

