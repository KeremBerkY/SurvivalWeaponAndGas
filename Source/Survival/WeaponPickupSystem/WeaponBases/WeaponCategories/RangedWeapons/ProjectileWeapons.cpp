// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapons.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "ProjectileWeapon/Projectiles/Projectile.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/CharacterAnimInstance.h"
#include "Survival/WeaponPickupSystem/Data/WeaponDataAssets/RangedWeaponData/ProjectileWeaponData/ProjectileWeaponData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/WeaponCategoriesUIHandlers/ProjectileWeaponUIHandler/ProjectileWeaponUIHandler.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/WeaponEffectManagerComponent/ProjectileEffectManagerComponent.h"


class UCharacterAnimInstance;

AProjectileWeapons::AProjectileWeapons()
{
	PrimaryActorTick.bCanEverTick = true;

	RearMuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("RearMuzzleLocation"));
	RearMuzzleLocation->SetupAttachment(GetWeaponMesh());

	EffectManagerComponent = CreateDefaultSubobject<UProjectileEffectManagerComponent>(TEXT("ProjectileEffectManagerComponent"));
	ProjectileWeaponUIHandler = CreateDefaultSubobject<UProjectileWeaponUIHandler>(TEXT("ProjectileWeaponUIHandler"));
}

void AProjectileWeapons::BeginPlay()
{
	Super::BeginPlay();

	if (UProjectileWeaponData* WeaponData = Cast<UProjectileWeaponData>(GetWeaponDataAsset()))
	{
		ProjectileWeaponDataAsset = WeaponData;
		OnProjectileEffectsInit.Broadcast();
	}

	SetCurrentAmmo(ProjectileWeaponDataAsset->WeaponAmmoAttributes.CurrentAmmo);
	
}

void AProjectileWeapons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileWeapons::PerformFire()
{
	Super::PerformFire();
}


void AProjectileWeapons::Attack()
{
	Super::Attack();
	UE_LOG(LogTemp, Warning, TEXT("Projectile Weapons Fire() Called!"));

	if(!CanFire()) return;
	
	if (ProjectileWeaponDataAsset->ProjectileClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FVector MuzzlePosition = GetMuzzleLocation()->GetComponentLocation();
			FRotator MuzzleRotation = GetMuzzleLocation()->GetComponentRotation();

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			OnProjectileFireMade.Broadcast();
			
			SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileWeaponDataAsset->ProjectileClass, MuzzlePosition, MuzzleRotation, SpawnParams);
			if (SpawnedProjectile)
			{
				SetProjectile(SpawnedProjectile);
				SpawnedProjectile->ProjectileInit(this);
				
				SpawnedProjectile->SetOwner(this);

				// SpawnedProjectile->ProjectileHit.AddUnique(SpawnedProjectile, &)
				
				// if (ProjectileWeaponDataAsset->ProjectileFireSpeed > 0.f)
				// {
				// 	
				// 	UProjectileMovementComponent* ProjectileMovementComponent = SpawnedProjectile->GetProjectileMovement();
				// 	ProjectileMovementComponent->InitialSpeed = ProjectileWeaponDataAsset->ProjectileFireSpeed;
				// }
				
				FVector LaunchDirection = MuzzleRotation.Vector();
				SpawnedProjectile->FireInDirection(LaunchDirection);
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AProjectileWeapons::ResetFire, ProjectileWeaponDataAsset->FireRate, false);

}

void AProjectileWeapons::EndAttack()
{
	Super::EndAttack();
}

void AProjectileWeapons::ResetFire()
{
	SetCanFire(true);
}

// void AProjectileWeapons::PlayWeaponEffect()
// {
// 	Super::PlayWeaponEffect();
//
// 	if (USceneComponent* MuzzleComponent = GetMuzzleLocation())
// 	{
// 		if (ProjectileWeaponData->ProjectileWeaponEffects.FrontMuzzleFlashEffect)
// 		{
// 			FVector EffectLocation = MuzzleComponent->GetComponentLocation() + MuzzleComponent->GetForwardVector();
// 			
// 			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileWeaponData->ProjectileWeaponEffects.FrontMuzzleFlashEffect, EffectLocation, MuzzleComponent->GetComponentRotation());
// 			UE_LOG(LogTemp, Warning, TEXT("Muzzle Flash effect triggered!"));
// 		}
//
// 		if (ProjectileWeaponData->ProjectileWeaponEffects.RearMuzzleEffect)
// 		{
// 			FVector EffectLocation = RearMuzzleLocation->GetComponentLocation() + RearMuzzleLocation->GetForwardVector();
//
// 			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileWeaponData->ProjectileWeaponEffects.RearMuzzleEffect, EffectLocation, RearMuzzleLocation->GetComponentRotation());
// 			UE_LOG(LogTemp, Warning, TEXT("REAR Muzzle Flash effect triggered!"));
//
// 		}
// 		
// 		if (ProjectileWeaponData->ProjectileWeaponEffects.TracerEffect)
// 		{
// 			FVector EffectLocation = MuzzleComponent->GetComponentLocation();
// 			FVector MuzzleDirection = MuzzleComponent->GetForwardVector();
// 			
// 			FVector TraceEnd = EffectLocation + (MuzzleDirection * 10000.f); // TODO: 10.000 yerine TrailEffectDistance yazabilirsin
//
// 			FHitResult HitResult;
// 			FCollisionQueryParams QueryParams;
// 			QueryParams.AddIgnoredActor(this);
// 			
// 			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EffectLocation, TraceEnd, ECC_Visibility, QueryParams);
//
// 			if (bHit)
// 			{
// 				TraceEnd = HitResult.ImpactPoint;
// 			}
// 			
// 			FRotator TracerRotation = (TraceEnd - EffectLocation).Rotation();
//
// 			if (UParticleSystemComponent* Tracer = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileWeaponData->ProjectileWeaponEffects.TracerEffect, EffectLocation, TracerRotation))
// 			{
// 				Tracer->SetFloatParameter(TEXT("Lifetime"), 0.3f);
// 			}
// 			UE_LOG(LogTemp, Warning, TEXT("Tracer effect triggered!"));
// 		}
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("MuzzleComponent is null. Effects cannot be triggered."));
// 	}
// }
//
// void AProjectileWeapons::ApplyRecoilEffect()
// {
	// if (!RecoilSettings.bCanApplyRecoil || !GetWeaponMesh()) return;
	//
	// Super::ApplyRecoilEffect();
	//
	// float ExtraRecoilAmount = RecoilSettings.BaseRecoilAmount * 1.5f;
	// float ExtraPitch = FMath::FRandRange(-ExtraRecoilAmount, -ExtraRecoilAmount * 0.8f);
	// float ExtraYaw = FMath::FRandRange(-ExtraRecoilAmount * 0.1f, ExtraRecoilAmount * 0.1f);
	//
	// FVector OriginalLocation = GetWeaponMesh()->GetRelativeLocation();
	// FRotator OriginalControlRotation;
	// APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//
	// if (PlayerController)
	// {
	// 	OriginalControlRotation = PlayerController->GetControlRotation();
	//
	// 	// Geri Tepme 
	// 	GetWeaponMesh()->SetRelativeLocation(OriginalLocation + FVector(-ExtraRecoilAmount, ExtraYaw, 0.0f));
	// 	FRotator NewControlRotation = OriginalControlRotation;
	// 	NewControlRotation.Pitch += ExtraPitch;
	// 	NewControlRotation.Yaw += ExtraYaw;
	// 	PlayerController->SetControlRotation(NewControlRotation);
	//
	// 	FTimerHandle TimerHandle;
	// 	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [PlayerController, OriginalControlRotation]()
	// 	{
	// 		if (PlayerController)
	// 		{
	// 			PlayerController->SetControlRotation(OriginalControlRotation);
	// 		}
	// 	}, 0.1f, false);
	// }
	//
	// FTimerHandle WeaponRecoilTimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(WeaponRecoilTimerHandle, [this, OriginalLocation]()
	// {
	// 	if (GetWeaponMesh())
	// 	{
	// 		GetWeaponMesh()->SetRelativeLocation(OriginalLocation);
	// 	}
	// }, 0.1f, false);
	//
	//
	// RecoilSettings.bCanApplyRecoil = false;
	// GetWorld()->GetTimerManager().SetTimer(RecoilSettings.RecoilCooldownTimerHandle, this, &ARangedWeapon::ResetRecoil, RecoilSettings.RecoilCooldown, false);
// }
