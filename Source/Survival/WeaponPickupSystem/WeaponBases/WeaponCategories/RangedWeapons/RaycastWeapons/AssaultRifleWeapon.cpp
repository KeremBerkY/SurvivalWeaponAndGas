// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultRifleWeapon.h"


#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/FireModesComponent/AutomaticShotModeComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/FireModesComponent/BurstShotModeComponent.h"


// Sets default values
AAssaultRifleWeapon::AAssaultRifleWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("AAssaultRifleWeapon constructor called"));

	BurstShotModeComponent = CreateDefaultSubobject<UBurstShotModeComponent>(TEXT("BurstShotModeComponent"));
	AutomaticShotModeComponent = CreateDefaultSubobject<UAutomaticShotModeComponent>(TEXT("AutomaticShotModeComponent"));
}


void AAssaultRifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAssaultRifleWeapon::AddFireModes()
{
	Super::AddFireModes();

	FireModeComponents.Add(BurstShotModeComponent);
	FireModeComponents.Add(AutomaticShotModeComponent);
	

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


void AAssaultRifleWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAssaultRifleWeapon::PerformFire()
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
	OnFirePlayMontage.Broadcast();
	
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


