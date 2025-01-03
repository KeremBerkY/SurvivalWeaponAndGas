// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterWeaponComponent.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/CharacterAnimInstance.h"
#include "Survival/WeaponPickupSystem/Character/CharacterTypes.h"
#include "Survival/WeaponPickupSystem/Character/Components/WeaponInventory.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/Interfaces/WeaponUIHandler.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponPickup.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponAttechmentManager/WeaponAttachmentManager.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/RaycastWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/HeatComponent/HeatComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/WeaponCategoriesUIHandlers/RaycastWeaponUIHandler/RaycastWeaponUIHandler.h"



// Sets default values for this component's properties
UCharacterWeaponComponent::UCharacterWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	WeaponAttachmentManager = NewObject<UWeaponAttachmentManager>();
	
	CharacterWeaponState = ECharacterWeaponStates::ECS_Unarmed;
	CurrentWeapon = nullptr;
}


void UCharacterWeaponComponent::UpdateCurrentWeaponUI() const
{
	if (GetCurrentWeapon())
	{
		if (const auto WeaponUIHandlerBaseComponent = GetCurrentWeapon()->GetWeaponUIHandlerBase())
		{
			if (const auto UIHandler = Cast<IWeaponUIHandler>(WeaponUIHandlerBaseComponent))
			{
				UIHandler->UpdateUI(GetCurrentWeapon());
			}
		}
	}
	
}

void UCharacterWeaponComponent::RemoveCurrentWeaponUI() const
{
	if (GetCurrentWeapon())
	{
		if (const auto WeaponUIHandlerBaseComponent = GetCurrentWeapon()->GetWeaponUIHandlerBase())
		{
			if (const auto UIHandler = Cast<IWeaponUIHandler>(WeaponUIHandlerBaseComponent))
			{
				UIHandler->RemoveUI(GetCurrentWeapon());
			}
		}
	}
	
}

void UCharacterWeaponComponent::BindUICallbacks(AWeaponBase* WeaponBase, ASurvivalCharacter* PlayerCharacter) const
{
	if (WeaponBase)
	{
		if (const auto WeaponUIHandlerBaseComponent = WeaponBase->GetWeaponUIHandlerBase())
		{
			if (const auto UIHandler = Cast<IWeaponUIHandler>(WeaponUIHandlerBaseComponent))
			{
				UIHandler->InitializeCallbacks(WeaponBase, PlayerCharacter);
			}
		}
	}
	
	
}

void UCharacterWeaponComponent::UpdateWeaponState(AWeaponBase* EquippedWeapon) // TODO: Bunu buradan kaldır! Delegate bağla
{
	if (!EquippedWeapon)
	{
		CharacterWeaponState = ECharacterWeaponStates::ECS_Unarmed;
		return;
	}
	
	switch (EquippedWeapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory)
	{
	case EWeaponCategory::Ewc_RaycastWeapons:
		CharacterWeaponState = ECharacterWeaponStates::ECS_CurrentWeaponRaycast;
		break;
	case EWeaponCategory::Ewc_ProjectileWeapons:
		CharacterWeaponState = ECharacterWeaponStates::ECS_CurrentWeaponProjectile;
		break;
	case EWeaponCategory::Ewc_MeleeWeapons:
		CharacterWeaponState = ECharacterWeaponStates::ECS_CurrentWeaponMelee;
		break;
	default:
		break;
	}
}

void UCharacterWeaponComponent::DropWeapon(const ASurvivalCharacter* PlayerCharacter, AWeaponBase* WeaponToDrop) // TODO: Inventory üstünden atılamıyor. O yüzden Weapon yollaman lazım.
{
	AWeaponBase* Weapon = (WeaponToDrop == nullptr) ? GetCurrentWeapon() : WeaponToDrop;
		if (Weapon)
		{
			if (const TSubclassOf<AWeaponPickup> WeaponPickupClass = Weapon->GetWeaponDataAsset()->WeaponAttributes.WeaponPickupClass)
			{
				const FVector DropLocation = PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorForwardVector() * 100.0f;
				const FRotator DropRotation = PlayerCharacter->GetActorRotation();

				if (const AWeaponPickup* WeaponPickup = GetWorld()->SpawnActor<AWeaponPickup>(WeaponPickupClass, DropLocation, DropRotation))
				{
					 if (UStaticMeshComponent* WeaponPickupStaticMeshComponent = WeaponPickup->GetStaticMeshComponent())
					 {
						WeaponPickupStaticMeshComponent->SetSimulatePhysics(true);
					 	WeaponPickupStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

						const FVector ForwardImpulse = PlayerCharacter->GetActorForwardVector() * DropDistance;
					 	WeaponPickupStaticMeshComponent->AddImpulse(ForwardImpulse);
					 }
					
					PlayerCharacter->GetWeaponInventory()->RemoveFromSlot(Weapon);
					Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					 if (GetCurrentWeapon() == Weapon)
					 {
					 	RemoveCurrentWeaponUI();
						SetCurrentWeapon(nullptr);
					 } 
					PlayerCharacter->GetCharacterAnimInstance()->UpdateWeaponType(GetCurrentWeapon()); // TODO: Delegate bağla, Broadcast(CurrentWeapon)
					Weapon->Destroy();
				}
			}
		}
	
}

bool UCharacterWeaponComponent::CanSwitchWeapon() const
{
	return (GetWorld()->TimeSeconds - LastSwitchTime) >= WeaponSwitchCooldown;
}

void UCharacterWeaponComponent::UpdateLastSwitchTime()
{
	LastSwitchTime = GetWorld()->TimeSeconds;
}

void UCharacterWeaponComponent::SpawnAndAddWeapon(const TSubclassOf<AWeaponBase>& WeaponInstance, ASurvivalCharacter* PlayerCharacter)
{
	if (!WeaponInstance) return;

	AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponInstance, PlayerCharacter->GetActorLocation(), PlayerCharacter->GetActorRotation());

	if (GetCurrentWeapon() == nullptr)
	{
		AddWeaponToCharacter(Weapon, PlayerCharacter);
		
	}
	else
	{
		PlayerCharacter->GetWeaponInventory()->AddWeaponToSlot(Weapon, PlayerCharacter);
		BindUICallbacks(Weapon, PlayerCharacter);
	}
}


void UCharacterWeaponComponent::AddWeaponToCharacter(AWeaponBase* NewWeapon, ASurvivalCharacter* PlayerCharacter)
{
	if (!NewWeapon) return;
			
	if (PlayerCharacter)
	{
		NewWeapon->SetOwner(PlayerCharacter);
		NewWeapon->SetOwningCharacter(PlayerCharacter);
		
		FName InSocketName = WeaponAttachmentManager->GetWeaponSocketName(this, NewWeapon);
		SetCurrentWeapon(NewWeapon);
		
		WeaponAttachmentManager->AttachMeshToSocket(PlayerCharacter->GetMesh(), InSocketName, NewWeapon);
		
		BindUICallbacks(NewWeapon, PlayerCharacter);
		UpdateCurrentWeaponUI();
	}

	if (UCharacterAnimInstance* AnimInstance = PlayerCharacter->GetCharacterAnimInstance())
	{
		AnimInstance->UpdateWeaponType(NewWeapon);
	}

}


void UCharacterWeaponComponent::EquipWeapon(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter, FName SocketName, bool bSetAsCurrent)
{
	if (Weapon)
	{
		RemoveCurrentWeaponUI();
		
		WeaponAttachmentManager->AttachWeaponToSocket(Weapon, PlayerCharacter, SocketName);
		
		// PlayerCharacter->GetCharacterWeaponComponent()->SetCurrentWeapon(bSetAsCurrent ? Weapon : nullptr);
		if (bSetAsCurrent)
		{
			SetCurrentWeapon(Weapon);
			Weapon->SetOwner(PlayerCharacter);
			Weapon->SetOwningCharacter(PlayerCharacter);
		}
		else
		{
			SetCurrentWeapon(nullptr);
		}
		
		
		UCharacterAnimInstance* AnimInstance = PlayerCharacter->GetCharacterAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->UpdateWeaponType(Weapon);
		}
		
		UpdateCurrentWeaponUI();
	}
}


