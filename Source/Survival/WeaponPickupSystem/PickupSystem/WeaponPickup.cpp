// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"

// #include "WeaponBase.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterWeaponComponent.h"
// #include "WeaponBases/WeaponBase.h"


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

void AWeaponPickup::Interact(AActor* Actor) //TODO: Yeni versiyon CharacterWeaponComponent hallediyor.
{
	if (!WeaponClass && !Actor) return;

	if (ASurvivalCharacter* PlayerCharacter = Cast<ASurvivalCharacter>(Actor))
	{
		PlayerCharacter->GetCharacterWeaponComponent()->AddWeapon(WeaponClass, PlayerCharacter);
	}
	Destroy();
}
