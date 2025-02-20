// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolWeapon.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/RaycastWeaponData/RaycastWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/FireModesComponent/FireModeBaseComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/HeatComponent/HeatComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/WeaponEffectManagerComponent/RaycastEffectManagerComponent.h"


APistolWeapon::APistolWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

}


void APistolWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void APistolWeapon::AddFireModes()
{
	Super::AddFireModes();

	if (FireModeComponents.Num() > 0)
	{
		CurrentFireModeIndex = 0;
		CurrentFireModeComponent = FireModeComponents[CurrentFireModeIndex];
		UE_LOG(LogTemp, Log, TEXT("Initialized fire mode: %s"), *CurrentFireModeComponent->GetClass()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No fire modes found on %s"), *GetName());
	}
}


void APistolWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APistolWeapon::PerformFire()
{
	Super::PerformFire();

	if (GetHeatComponent()->GetCurrentHeat() >= GetRaycastWeaponDataAsset()->FiringHeatSettings.MaxHeatCapacity || GetHeatComponent()->IsOverHeated() ) { return; }
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());

	OnFireMade.Broadcast();
	EffectManagerComponent->ApplyRecoilEffect();
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		RaycastMuzzleLocation,
		RaycastTraceEnd,
		ECC_Visibility,
		QueryParams
		);

	if (bHit)
	{
		OnImpact.Broadcast(HitResult);

		if (AActor* HitActor = HitResult.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("ApplyPointDamage()"));
			// UGameplayStatics::ApplyPointDamage(HitActor, Damage, (End - Start).GetSafeNormal(), HitResult, GetOwnerController(), this, DamageType);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Raycast missed. No hit detected."));
	}
	
	DrawDebugVisuals(RaycastMuzzleLocation, RaycastTraceEnd, HitResult);
}



