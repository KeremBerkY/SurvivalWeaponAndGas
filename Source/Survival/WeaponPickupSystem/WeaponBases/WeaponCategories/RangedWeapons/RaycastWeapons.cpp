// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastWeapons.h"

#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/WeaponCategoriesUIHandlers/RaycastWeaponUIHandler/RaycastWeaponUIHandler.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/FireModesComponent/SingleShotModeComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/HeatComponent/HeatComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/WeaponAnimationsComponent/WeaponAnimationsComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/WeaponEffectManagerComponent/RaycastEffectManagerComponent.h"


ARaycastWeapons::ARaycastWeapons()
{
	PrimaryActorTick.bCanEverTick = true;
	
	EffectManagerComponent = CreateDefaultSubobject<URaycastEffectManagerComponent>(TEXT("WeaponEffectManagerComponent"));
	WeaponAnimationsComponent = CreateDefaultSubobject<UWeaponAnimationsComponent>(TEXT("WeaponAnimationsComponent"));
	HeatComponent = CreateDefaultSubobject<UHeatComponent>(TEXT("HeatComponent"));
	RaycastWeaponUIHandler = CreateDefaultSubobject<URaycastWeaponUIHandler>(TEXT("RaycastWeaponUIHandler"));
	
	SingleShotModeComponent = CreateDefaultSubobject<USingleShotModeComponent>(TEXT("SingleShotModeComponent"));
}

void ARaycastWeapons::BeginPlay()
{
	Super::BeginPlay();

	if (URaycastWeaponData* WeaponData = Cast<URaycastWeaponData>(GetWeaponDataAsset()))
	{
		RaycastWeaponData = WeaponData;
		OnInitializedComponents.Broadcast();
	}

	SetCurrentAmmo(RaycastWeaponData->WeaponAmmoAttributes.CurrentAmmo);
	
	AddFireModes();
}

void ARaycastWeapons::AddFireModes()
{
		FireModeComponents.Add(SingleShotModeComponent);
	
}

void ARaycastWeapons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARaycastWeapons::Attack()
{
	Super::Attack();

	if (GetCurrentAmmo() <= 0 || !CanFire() || HeatComponent->GetCurrentHeat() >= RaycastWeaponData->FiringHeatSettings.MaxHeatCapacity || HeatComponent->IsOverHeated()) return;
	
	// if (GetActiveFireMode())
	// {
	// 	UpdateHeat.Broadcast();
	// 	GetActiveFireMode()->Fire();
	// 	DecreaseCurrentAmmo();
	// 	
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("ActiveFireMode is nullptr!"));
	// }

	if (CurrentFireModeComponent && CanFire())
	{
		CurrentFireModeComponent->Fire();
	}
}

void ARaycastWeapons::EndAttack()
{
	Super::EndAttack();

	if (CurrentFireModeComponent)
	{
		CurrentFireModeComponent->EndFire();
	}
	
}


void ARaycastWeapons::PerformFire()
{
	Super::PerformFire();

	if (GetCurrentAmmo() <= 0 || HeatComponent->GetCurrentHeat() >= RaycastWeaponData->FiringHeatSettings.MaxHeatCapacity || HeatComponent->IsOverHeated()) return;
	
	UpdateHeat.Broadcast();
	DecreaseCurrentAmmo();
}

void ARaycastWeapons::DrawDebugVisuals(const FVector& Start, const FVector& End, const FHitResult& HitResult) const
{
	// Raycast çizgisi
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
	
	// Çarpma noktası üzerinde bir küre çiz
	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 12, FColor::Green, false, 1.0f);
	
		// Çarpılan yerin adını logla
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s, Impact Point: %s"), *HitActor->GetName(), *HitResult.ImpactPoint.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No actor hit. Impact Point: %s"), *HitResult.ImpactPoint.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Raycast did not hit any surface."));
	}
}


