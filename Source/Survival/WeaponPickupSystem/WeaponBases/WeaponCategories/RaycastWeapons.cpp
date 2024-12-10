// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastWeapons.h"

#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/CharacterAnimInstance.h"
#include "Survival/WeaponPickupSystem/Character/Components/CharacterCameraComponent.h"
#include "Survival/WeaponPickupSystem/WeaponBases/FireModes/BurstShotFireMode.h"


ARaycastWeapons::ARaycastWeapons()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARaycastWeapons::InitializeFireModes()
{
	Super::InitializeFireModes();

	TScriptInterface<IFireMode> BurstFireMode;
	BurstFireMode.SetObject(NewObject<UBurstShotFireMode>(this));
	BurstFireMode.SetInterface(Cast<IFireMode>(BurstFireMode.GetObject()));
	FireModeMap.Add(EFireMode::BurstFire, BurstFireMode);
}


void ARaycastWeapons::BeginPlay()
{
	Super::BeginPlay();

	InitializeFireModes();

}

void ARaycastWeapons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARaycastWeapons::Fire()
{
	Super::Fire();

	FVector LocalMuzzleLocation = GetMuzzleLocation()->GetComponentLocation();
	FVector MuzzleForward = GetMuzzleLocation()->GetForwardVector();
	FVector TraceEnd = LocalMuzzleLocation + (MuzzleForward * 10000.0f);

	UE_LOG(LogTemp, Warning, TEXT("Fire() called. Muzzle location: %s, Trace end: %s"), *LocalMuzzleLocation.ToString(), *TraceEnd.ToString());

	FireAtTarget(LocalMuzzleLocation, TraceEnd);

	// Debug çizgileri
	DrawDebugLine(GetWorld(), LocalMuzzleLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);
}

void ARaycastWeapons::FireAtTarget(const FVector& Start, const FVector& End)
{
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams
		);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Raycast hit! Impact point: %s, Actor: %s"), *HitResult.ImpactPoint.ToString(), *HitResult.GetActor()->GetName());

		PlayImpactEffect(HitResult);

		ApplyDecal(HitResult);

		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("ApplyPointDamage()"));
			// UGameplayStatics::ApplyPointDamage(HitActor, Damage, (End - Start).GetSafeNormal(), HitResult, GetOwnerController(), this, DamageType);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Raycast missed. No hit detected."));
	}
	
	DrawDebugVisuals(Start, End, HitResult);
}

void ARaycastWeapons::ApplyDecal(const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ApplyDecal() called. Impact point: %s, Impact normal: %s"), *HitResult.ImpactPoint.ToString(), *HitResult.ImpactNormal.ToString());

	if (DecalSettings.DecalMaterial)
	{
		UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			DecalSettings.DecalMaterial,
			DecalSettings.DecalSize,
			HitResult.ImpactPoint,
			HitResult.ImpactNormal.Rotation(),
			DecalSettings.DecalLifeSpan
		);

		if (Decal)
		{
			UE_LOG(LogTemp, Warning, TEXT("Decal successfully spawned."));
			Decal->SetFadeOut(DecalSettings.DecalLifeSpan, 2.0);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Decal spawn failed."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Decal Material assigned."));
	}
}

void ARaycastWeapons::PlayWeaponEffect()
{
	Super::PlayWeaponEffect();
	
	USceneComponent* MuzzleComponent = GetMuzzleLocation();
	if (!MuzzleComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("MuzzleComponent is null. Effects cannot be triggered."));
		return;
	}

	if (MuzzleFlashEffect)
	{
		FVector EffectLocation = MuzzleComponent->GetComponentLocation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashEffect, EffectLocation, MuzzleComponent->GetComponentRotation());
	}
}

void ARaycastWeapons::PlayImpactEffect(const FHitResult& HitResult)
{
	if (ImpactEffect)
	{
		FVector ImpactLocation = HitResult.ImpactPoint;
		FRotator ImpactRotation = HitResult.ImpactNormal.Rotation();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, ImpactLocation, ImpactRotation);
	}
}

void ARaycastWeapons::ApplyRecoilEffect()
{
	Super::ApplyRecoilEffect();
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

