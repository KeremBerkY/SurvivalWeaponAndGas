// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterWeaponComponent.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/CharacterAnimInstance.h"
#include "Survival/WeaponPickupSystem/Character/CharacterTypes.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponBase.h"


// Sets default values for this component's properties
UCharacterWeaponComponent::UCharacterWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CharacterWeaponState = ECharacterWeaponStates::ECS_Unarmed;
	CurrentWeapon = nullptr;
}

void UCharacterWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UCharacterWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCharacterWeaponComponent::UpdateWeaponState(AWeaponBase* EquippedWeapon)
{
	if (!EquippedWeapon)
	{
		CharacterWeaponState = ECharacterWeaponStates::ECS_Unarmed;
		return;
	}
	
	switch (EquippedWeapon->WeaponCategory)
	{
	case EWeaponCategories::EWC_RaycastWeapons:
		CharacterWeaponState = ECharacterWeaponStates::ECS_CurrentWeaponRaycast;
		break;
	case EWeaponCategories::EWC_ProjectileWeapons:
		CharacterWeaponState = ECharacterWeaponStates::ECS_CurrentWeaponProjectile;
		break;
	case EWeaponCategories::EWC_MeleeWeapons:
		CharacterWeaponState = ECharacterWeaponStates::ECS_CurrentWeaponMelee;
		break;
	default:
		break;
	}
}

void UCharacterWeaponComponent::DropWeapon(ASurvivalCharacter* PlayerCharacter)
{
	if (CurrentWeapon)
	{
		AWeaponBase* WeaponDrop = Cast<AWeaponBase>(CurrentWeapon); // TODO: Bu Cast işlemi gereksiz olabilir. Sistemi güncelleyince SİL!
		if (WeaponDrop)
		{
			WeaponDrop->DropWeapon(PlayerCharacter);
			CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurrentWeapon = nullptr;
			PlayerCharacter->GetCharacterAnimInstance()->UpdateWeaponType(CurrentWeapon); // TODO: Delegate bağla, Broadcast(CurrentWeapon)
		}
	}
}

