// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolWeapon.h"

#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/WeaponEffectManagerComponent/RaycastEffectManagerComponent.h"


APistolWeapon::APistolWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

}


void APistolWeapon::BeginPlay()
{
	Super::BeginPlay();

}



void APistolWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APistolWeapon::PerformFire()
{
	Super::PerformFire();

	FVector LocalMuzzleLocation = GetMuzzleLocation()->GetComponentLocation();
	FVector MuzzleForward = GetMuzzleLocation()->GetForwardVector();
	FVector TraceEnd = LocalMuzzleLocation + (MuzzleForward * 10000.0f);
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());

	OnFireMade.Broadcast();
	EffectManagerComponent->ApplyRecoilEffect();
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		LocalMuzzleLocation,
		TraceEnd,
		ECC_Visibility,
		QueryParams
		);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Raycast hit! Impact point: %s, Actor: %s"), *HitResult.ImpactPoint.ToString(), *HitResult.GetActor()->GetName());
		
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
	
	DrawDebugVisuals(LocalMuzzleLocation, TraceEnd, HitResult);
}



