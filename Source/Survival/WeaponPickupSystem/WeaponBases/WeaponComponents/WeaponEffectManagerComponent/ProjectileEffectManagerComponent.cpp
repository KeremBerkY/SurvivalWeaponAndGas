// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffectManagerComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Survival/WeaponPickupSystem/Data/ProjectileDataAssets/ProjectileData.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/ProjectileWeaponData/ProjectileWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/ProjectileWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/ProjectileWeapon/Projectiles/Projectile.h"


UProjectileEffectManagerComponent::UProjectileEffectManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UProjectileEffectManagerComponent::BeginPlay()
{
	Super::BeginPlay();

		
	Weapon = Cast<AProjectileWeapons>(GetOwner());
	if (Weapon)
	{
		Weapon->OnProjectileEffectsInit.AddDynamic(this, &UProjectileEffectManagerComponent::InitializeEffects);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no Weapon! Weapon = nullptr."));
	}
	
}

void UProjectileEffectManagerComponent::InitializeEffects()
{
	ProjectileWeaponData = Weapon->GetProjectileWeaponDataAsset();

	if (!ProjectileWeaponData) return;
	Weapon->OnProjectileFireMade.AddDynamic(this, &UProjectileEffectManagerComponent::PlayWeaponEffect);
	// Weapon->GetProjectile()->OnProjectileInitialize.AddDynamic(this, &UProjectileEffectManagerComponent::InitializeProjectile);
}

void UProjectileEffectManagerComponent::InitializeProjectile(AProjectile* SpawnedProjectile)
{
	Projectile = SpawnedProjectile;
	if (Projectile)
	{
		ProjectileData = Projectile->GetProjectileData();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no Projectile!"))
	}

	if (!ProjectileData) return; // TODO: Burada patlıyoruz buna bak ve esnek bir şekilde bağla bu denediğin stil kötü!
	Projectile->OnExplosion.AddDynamic(this, &UProjectileEffectManagerComponent::HandleExplosion);
}

void UProjectileEffectManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UProjectileEffectManagerComponent::PlayWeaponEffect()
{

	if (USceneComponent* MuzzleComponent = Weapon->GetMuzzleLocation())
	{
		if (ProjectileWeaponData->ProjectileWeaponEffects.FrontMuzzleFlashEffect)
		{
			FVector EffectLocation = MuzzleComponent->GetComponentLocation() + MuzzleComponent->GetForwardVector();
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileWeaponData->ProjectileWeaponEffects.FrontMuzzleFlashEffect, EffectLocation, MuzzleComponent->GetComponentRotation());
			UE_LOG(LogTemp, Warning, TEXT("Muzzle Flash effect triggered!"));
		}

		if (ProjectileWeaponData->ProjectileWeaponEffects.RearMuzzleEffect)
		{
			FVector EffectLocation = Weapon->GetRearMuzzleLocation()->GetComponentLocation() + Weapon->GetRearMuzzleLocation()->GetForwardVector();

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileWeaponData->ProjectileWeaponEffects.RearMuzzleEffect, EffectLocation, Weapon->GetRearMuzzleLocation()->GetComponentRotation());
			UE_LOG(LogTemp, Warning, TEXT("REAR Muzzle Flash effect triggered!"));

		}
		
		if (ProjectileWeaponData->ProjectileWeaponEffects.TracerEffect)
		{
			FVector EffectLocation = MuzzleComponent->GetComponentLocation();
			FVector MuzzleDirection = MuzzleComponent->GetForwardVector();
			
			FVector TraceEnd = EffectLocation + (MuzzleDirection * 10000.f); // TODO: 10.000 yerine TrailEffectDistance yazabilirsin

			FHitResult HitResult;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(Weapon);
			
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EffectLocation, TraceEnd, ECC_Visibility, QueryParams);

			if (bHit)
			{
				TraceEnd = HitResult.ImpactPoint;
			}
			
			FRotator TracerRotation = (TraceEnd - EffectLocation).Rotation();

			if (UParticleSystemComponent* Tracer = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileWeaponData->ProjectileWeaponEffects.TracerEffect, EffectLocation, TracerRotation))
			{
				Tracer->SetFloatParameter(TEXT("Lifetime"), 0.3f);
			}
			UE_LOG(LogTemp, Warning, TEXT("Tracer effect triggered!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MuzzleComponent is null. Effects cannot be triggered."));
	}
}

void UProjectileEffectManagerComponent::HandleExplosion()
{
	// TODO: SpawnEmitterAtLocation ile patlama efekti yap!

	if (ProjectileData->ProjectileSettings.HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ProjectileData->ProjectileSettings.HitEffect,
			Projectile->GetActorLocation(),
			FRotator::ZeroRotator,
			true
		);
	}
	
	if (ProjectileData->ExplosiveSettings.ExplosionRadius > 0.f)
	{
		// UGameplayStatics::ApplyRadialDamage(
		// 	GetWorld(),
		// 	ProjectileData->ExplosiveSettings.ExplosiveDamage,
		// 	Projectile->GetActorLocation(),
		// 	ProjectileData->ExplosiveSettings.ExplosionRadius,
		// 	nullptr,
		// 	TArray<AActor*>(),
		// 	Projectile,
		// 	Projectile->GetInstigatorController(),
		// 	true
		// 	);

		UE_LOG(LogTemp, Warning, TEXT("Explosion called!"));
		DrawDebugSphere(GetWorld(), Projectile->GetActorLocation(), 150.f, 12, FColor::Red, false, 2.0f);

		Projectile->Destroy();
	}
}

