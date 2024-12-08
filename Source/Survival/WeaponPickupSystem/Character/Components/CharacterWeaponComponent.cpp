// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterWeaponComponent.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/CharacterAnimInstance.h"
#include "Survival/WeaponPickupSystem/Character/CharacterTypes.h"
#include "Survival/WeaponPickupSystem/Character/WeaponInventory.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponPickup.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponBase.h"
#include "Survival/WeaponPickupSystem/PickupSystem/WeaponBases/WeaponAttechmentManager/WeaponAttachmentManager.h"


// Sets default values for this component's properties
UCharacterWeaponComponent::UCharacterWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	WeaponAttachmentManager = NewObject<UWeaponAttachmentManager>();
	
	CharacterWeaponState = ECharacterWeaponStates::ECS_Unarmed;
	CurrentWeapon = nullptr;
}

void UCharacterWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UCharacterWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

void UCharacterWeaponComponent::DropWeapon(ASurvivalCharacter* PlayerCharacter, AWeaponBase* WeaponToDrop) // TODO: Inventory üstünden atılamıyor. O yüzden Weapon yollaman lazım.
{
	AWeaponBase* Weapon = (WeaponToDrop == nullptr) ? GetCurrentWeapon() : WeaponToDrop;
		if (Weapon)
		{
			if (TSubclassOf<AWeaponPickup> WeaponPickupClass = Weapon->GetWeaponPickupClass())
			{
				FVector DropLocation = PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorForwardVector() * 100.0f;
				FRotator DropRotation = PlayerCharacter->GetActorRotation();

				if (AWeaponPickup* WeaponPickup = GetWorld()->SpawnActor<AWeaponPickup>(WeaponPickupClass, DropLocation, DropRotation))
				{
					 if (UStaticMeshComponent* WeaponPickupStaticMeshComponent = WeaponPickup->GetStaticMeshComponent())
					 {
						WeaponPickupStaticMeshComponent->SetSimulatePhysics(true);
					 	WeaponPickupStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

					 	FVector ForwardImpulse = PlayerCharacter->GetActorForwardVector() * DropDistance;
					 	WeaponPickupStaticMeshComponent->AddImpulse(ForwardImpulse);
					 }

					PlayerCharacter->GetWeaponInventory()->RemoveFromSlot(Weapon);
					Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					Weapon->Destroy();
					 if (GetCurrentWeapon() == Weapon)
					 {
						SetCurrentWeapon(nullptr);
					 }
					PlayerCharacter->GetCharacterAnimInstance()->UpdateWeaponType(GetCurrentWeapon()); // TODO: Delegate bağla, Broadcast(CurrentWeapon)
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

void UCharacterWeaponComponent::AddWeaponToCharacter(AWeaponBase* NewWeapon, ASurvivalCharacter* PlayerCharacter)
{
	if (!NewWeapon) return;
			
	if (PlayerCharacter)
	{
		FName InSocketName = WeaponAttachmentManager->GetWeaponSocketName(this, NewWeapon);
		SetCurrentWeapon(NewWeapon);
		
		WeaponAttachmentManager->AttachMeshToSocket(PlayerCharacter->GetMesh(), InSocketName, NewWeapon);
	}
			
	UCharacterAnimInstance* AnimInstance = PlayerCharacter->GetCharacterAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->UpdateWeaponType(NewWeapon);
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance UpdateWeapon called !"))
	}
	// OnWeaponUpdated.Broadcast(NewWeapon);
}

// void UCharacterWeaponComponent::EquipWeapon(AWeaponBase* NewWeapon, ASurvivalCharacter* PlayerCharacter) // WeaponBase den aldın bu EquipWeapon'u
// {
//
// 	if (!NewWeapon) return;
// 	
// 	if (PlayerCharacter)
// 	{
// 		FName InSocketName = NewWeapon->GetWeaponSocketName(PlayerCharacter);
// 		SetCurrentWeapon(NewWeapon);
// 	
// 		NewWeapon->AttechMeshToSocket(PlayerCharacter->GetMesh(), InSocketName);		
// 	}
// 	
// 	UCharacterAnimInstance* AnimInstance = PlayerCharacter->GetCharacterAnimInstance();
// 	if (AnimInstance)
// 	{
// 		AnimInstance->UpdateWeaponType(NewWeapon);
// 		UE_LOG(LogTemp, Warning, TEXT("AnimInstance UpdateWeapon called !"))
// 	}
// 	// OnWeaponUpdated.Broadcast(NewWeapon);
// }

// DENEME 2.
// void UCharacterWeaponComponent::EquipWeapon(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter, bool bFromInventory, FName CustomSocketName = NAME_None) 
// {
// 	if (!Weapon || !PlayerCharacter) return;
//
// 	// Socket adını belirle
// 	FName SocketName = (CustomSocketName != NAME_None)
// 		? CustomSocketName
// 		: Weapon->GetWeaponSocketName(PlayerCharacter);
// 	
//
// 	UE_LOG(LogTemp, Warning, TEXT("EquipWeapon called! SocketName: %s"), *SocketName.ToString());
// 	
// 	// Silah kategorisine göre işleme devam et
// 	switch (Weapon->WeaponCategory)
// 	{
// 	case EWeaponCategories::EWC_RaycastWeapons:
// 	case EWeaponCategories::EWC_ProjectileWeapons:
// 		Weapon->AttechMeshToSocket(PlayerCharacter->GetMesh(), SocketName);
// 		break;
//
// 	case EWeaponCategories::EWC_MeleeWeapons:
// 		UE_LOG(LogTemp, Warning, TEXT("Melee Weapon equipped to socket: %s"), *SocketName.ToString());
// 		break;
//
// 	case EWeaponCategories::EWC_MAX:
// 	default:
// 		UE_LOG(LogTemp, Warning, TEXT("Invalid WeaponCategory for Weapon: %s"), *Weapon->GetName());
// 		return;
// 	}
//
// 	// PlayerCharacter->GetCharacterWeaponComponent()->SetCurrentWeapon(bSetAsCurrent ? Weapon : nullptr);
// 	if (CustomSocketName != NAME_None)
// 	{
// 		SetCurrentWeapon(Weapon);
// 	}
// 	
//
// 	// Animasyon güncellemesi
// 	UCharacterAnimInstance* AnimInstance = PlayerCharacter->GetCharacterAnimInstance();
// 	if (AnimInstance)
// 	{
// 		AnimInstance->UpdateWeaponType(Weapon);
// 	}
//
// 	// Event tetikle
// 	OnWeaponUpdated.Broadcast(Weapon);
//
// 	UE_LOG(LogTemp, Warning, TEXT("Equipped weapon: %s to socket: %s"), *Weapon->GetName(), *SocketName.ToString());
// }

void UCharacterWeaponComponent::EquipWeapon(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter, FName SocketName, bool bSetAsCurrent) // TODO: Bunu CharacterWeaponComponent'a taşı
{
	if (Weapon)
	{
		// PlayerCharacter->GetWeaponInventory()->AttachWeaponToSocket(Weapon, PlayerCharacter, SocketName);
		WeaponAttachmentManager->AttachWeaponToSocket(Weapon, PlayerCharacter, SocketName);
		
		PlayerCharacter->GetCharacterWeaponComponent()->SetCurrentWeapon(bSetAsCurrent ? Weapon : nullptr);
		
		UCharacterAnimInstance* AnimInstance = PlayerCharacter->GetCharacterAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->UpdateWeaponType(Weapon);
		}

		UE_LOG(LogTemp, Warning, TEXT("Equipped weapon: %s to socket: %s"), *Weapon->GetName(), *SocketName.ToString());
	}
}

//
//
void UCharacterWeaponComponent::AddWeapon(const TSubclassOf<AWeaponBase>& WeaponInstance, ASurvivalCharacter* PlayerCharacter)
{
	if (!WeaponInstance) return;

	AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponInstance, PlayerCharacter->GetActorLocation(), PlayerCharacter->GetActorRotation());

	if (GetCurrentWeapon() == nullptr)
	{
		// EquipWeapon(Weapon, PlayerCharacter, false, "WeaponSocket");
		AddWeaponToCharacter(Weapon, PlayerCharacter);
	}
	else
	{
		PlayerCharacter->GetWeaponInventory()->AddWeaponToSlot(Weapon, PlayerCharacter);
	}
}

