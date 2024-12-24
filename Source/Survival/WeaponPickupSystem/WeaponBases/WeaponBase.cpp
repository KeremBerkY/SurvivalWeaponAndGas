// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "WeaponCategories/WeaponCategoriesUIHandlers/WeaponUIHandlerBase.h"


AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	// WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	bIsAttackCooldownActive = false;

}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	WeaponUIHandlerBase = FindComponentByClass<UWeaponUIHandlerBase>();
	if (!WeaponUIHandlerBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("No WeaponUIHandler component found on %s"), *GetName());
	}
	
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::Attack()
{
}

void AWeaponBase::EndAttack()
{
}


void AWeaponBase::PlayWeaponEffect()
{
	if (BaseEffect)
	{
		// TODO: Burada hepsinde bulunacak olan bir efekti spawn edebilirsin...
	}
}
