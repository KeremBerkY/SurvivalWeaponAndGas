// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAttachmentManager.h"

#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponBase.h"

FName UWeaponAttachmentManager::GetWeaponSocketName(UCharacterWeaponComponent* WeaponComponent, AWeaponBase* Weapon) // TODO: Bunu CharacterWeaponComponent'a taşıma !! bu silahın socket ismini alıyor!!
{
	// AWeaponBase* CurrentWeapon = PlayerCharacter->GetCurrentWeapon();
	AWeaponBase* CurrentWeapon = WeaponComponent->GetCurrentWeapon();
	FName Socketname;
	if (CurrentWeapon == nullptr)
	{
		Socketname = "WeaponSocket";
		UE_LOG(LogTemp, Error, TEXT("CurrentWeapon == nullptr: FName = WeaponSocket"));
		WeaponComponent->UpdateWeaponState(Weapon);
		UE_LOG(LogTemp, Error, TEXT("UpdateCharacterState()"));

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GetWeaponSocketName called!! Switch Case"));

		switch (Weapon->WeaponType)
		{
		case EWeaponType::Ewt_AssaultRifle:
			Socketname = "RaycastSocket";
			break;
		case EWeaponType::Ewt_Pistol:
			Socketname = "RaycastPistolSocket";
			break;
		case EWeaponType::Ewt_Shotgun:
			Socketname = "RaycastSocket";
			break;
		case EWeaponType::EWT_GrenadeLauncher:
			Socketname = "ProjectileSocket";
			break;
		case EWeaponType::Ewt_RocketLauncher:
			Socketname = "ProjectileSocket";
			break;
		case EWeaponType::EWT_Sword:
			Socketname = "MeleeSocket";
			break;
		case EWeaponType::EWT_Axe:
			Socketname = "MeleeSocket";
			break;
		default:
			Socketname = FName("DefaultWeaponSocket");
			break;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("SocketName: %s"), *Socketname.ToString());
	return Socketname;
}

void UWeaponAttachmentManager::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName, AWeaponBase* Weapon)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	Weapon->AttachToComponent(InParent, TransformRules, InSocketName);
}

void UWeaponAttachmentManager::AttachWeaponToSocket(AWeaponBase* NewWeapon, ASurvivalCharacter* PlayerCharacter, FName CustomSocketName)
{
	if (!NewWeapon || !PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttachWeaponToSocket called! but there is no Weapon!!"))
		return;
	}
	
	FName SocketName = (CustomSocketName != NAME_None)
		? CustomSocketName
		: GetWeaponSocketName(PlayerCharacter->GetCharacterWeaponComponent(), NewWeapon); // TODO: NewWeapon'u argüman olarak göndermelisin.
	
	UE_LOG(LogTemp, Warning, TEXT("AttachWeaponToSocket called! SocketName: %s"), *SocketName.ToString())
	
	// FName SocketName = NewWeapon->GetWeaponSocketName(PlayerCharacter);
	switch (NewWeapon->WeaponCategory)
	{
	case EWeaponCategories::EWC_RaycastWeapons:
		AttachMeshToSocket(PlayerCharacter->GetMesh(), SocketName, NewWeapon);
		break;
	case EWeaponCategories::EWC_ProjectileWeapons:
		AttachMeshToSocket(PlayerCharacter->GetMesh(), SocketName, NewWeapon);
		break;
	case EWeaponCategories::EWC_MeleeWeapons:
		UE_LOG(LogTemp, Warning, TEXT("Melee Weapon added to socket"))
		break;
	case EWeaponCategories::EWC_MAX:
		UE_LOG(LogTemp, Warning, TEXT("Cannot add to socket! Empty NewWeapon Category!!"))
		break;
	default: ;
	}
}