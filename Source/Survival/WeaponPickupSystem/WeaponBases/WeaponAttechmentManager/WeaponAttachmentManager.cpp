// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAttachmentManager.h"

#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/WeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"

FName UWeaponAttachmentManager::GetWeaponSocketName(UCharacterWeaponComponent* WeaponComponent, AWeaponBase* Weapon) // TODO: Bunu CharacterWeaponComponent'a taşıma !! bu silahın socket ismini alıyor!!
{
	// AWeaponBase* CurrentWeapon = PlayerCharacter->GetCurrentWeapon();
	AWeaponBase* CurrentWeapon = WeaponComponent->GetCurrentWeapon();
	FName Socketname;
	if (CurrentWeapon == nullptr)
	{
		Socketname = "WeaponSocket";
		UE_LOG(LogTemp, Warning, TEXT("CurrentWeapon == nullptr: FName = WeaponSocket"));
		WeaponComponent->UpdateWeaponState(Weapon);
		UE_LOG(LogTemp, Warning, TEXT("UpdateCharacterState()"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWeaponSocketName called!! Switch Case"));

		
		switch (Weapon->GetWeaponDataAsset()->WeaponAttributes.WeaponTypes)
		{
		case EWeaponTypes::Ewt_AssaultRifle:
			Socketname = "RaycastSocket";
			break;
		case EWeaponTypes::Ewt_Pistol:
			Socketname = "RaycastPistolSocket";
			break;
		case EWeaponTypes::Ewt_Shotgun:
			Socketname = "RaycastSocket";
			break;
		case EWeaponTypes::Ewt_GrenadeLauncher:
			Socketname = "ProjectileSocket";
			break;
		case EWeaponTypes::Ewt_RocketLauncher:
			Socketname = "ProjectileSocket";
			break;
		case EWeaponTypes::Ewt_Sword:
			Socketname = "MeleeSocket";
			break;
		case EWeaponTypes::Ewt_Axe:
			Socketname = "MeleeSocket";
			break;
		default:
			Socketname = FName("DefaultWeaponSocket");
			break;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("SocketName: %s"), *Socketname.ToString());
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
	switch (NewWeapon->GetWeaponDataAsset()->WeaponAttributes.WeaponCategory)
	{
	case EWeaponCategory::Ewc_RaycastWeapons:
		AttachMeshToSocket(PlayerCharacter->GetMesh(), SocketName, NewWeapon);
		break;
	case EWeaponCategory::Ewc_ProjectileWeapons:
		AttachMeshToSocket(PlayerCharacter->GetMesh(), SocketName, NewWeapon);
		break;
	case EWeaponCategory::Ewc_MeleeWeapons:
		UE_LOG(LogTemp, Warning, TEXT("Melee Weapon added to socket"))
		break;
	case EWeaponCategory::Ewc_Max:
		UE_LOG(LogTemp, Warning, TEXT("Cannot add to socket! Empty NewWeapon Category!!"))
		break;
	default: ;
	}
}