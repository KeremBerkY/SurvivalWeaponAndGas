// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"

// #include "WeaponBase.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/WeaponInventory.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
#include "WeaponBases/WeaponBase.h"


AWeaponPickup::AWeaponPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	// PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	// RootComponent = PickupMesh;
}

void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponPickup::Interact(AActor* Actor)
{
	if (GetWeaponClass())
	{
		AWeaponBase* WeaponInstance = GetWorld()->SpawnActor<AWeaponBase>(GetWeaponClass(), GetActorLocation(), GetActorRotation());
		ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(Actor);
		if (WeaponInstance && PlayerCharacter)
		{
			// Bu kısımda sadece CurrentWeapon check ettim ve if/else bloku yazdım
			// AWeaponBase* CurrentWeapon = PlayerCharacter->GetCurrentWeapon();
			AWeaponBase* CurrentWeapon = PlayerCharacter->GetCharacterWeaponComponent()->GetCurrentWeapon();
			if (CurrentWeapon == nullptr)
			{
				WeaponInstance->Equip(PlayerCharacter); // ? UWeaponInventory::EquipWeapon() daki fonksiyonu CharacterWeaponComponent taşıyabiliriz. if (CurrentWeapon == nullptr) ise EquipWeapon(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter, FName SocketName, bool bSetAsCurrent = true) SocketName parametresine UWeaponInventory de bulunan WeaponSocket değerini verirsen set edebilirsin.
				UE_LOG(LogTemp, Warning, TEXT("WeaponInstance->Equip()"));
			}
			else // WeaponInventory hallediyor
			{
				PlayerCharacter->GetWeaponInventory()->AddWeaponToSlot(WeaponInstance, PlayerCharacter);
				UE_LOG(LogTemp, Warning, TEXT("AddWeaponToSlot()"));
			}
		}
	}
	Destroy();
}
