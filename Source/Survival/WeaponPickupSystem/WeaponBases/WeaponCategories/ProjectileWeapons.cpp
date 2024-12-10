// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapons.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "ProjectileWeapon/Projectiles/Projectile.h"
#include "Survival/SurvivalCharacter.h"
#include "Survival/WeaponPickupSystem/Character/CharacterAnimInstance.h"


class UCharacterAnimInstance;

AProjectileWeapons::AProjectileWeapons()
{
	PrimaryActorTick.bCanEverTick = true;

	RearMuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("RearMuzzleLocation"));
	RearMuzzleLocation->SetupAttachment(GetWeaponMesh());
}

void AProjectileWeapons::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileWeapons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void AProjectileWeapons::Fire()
{
	Super::Fire();
	UE_LOG(LogTemp, Error, TEXT("Projectile Weapons Fire() Called!"));
	
	if (ProjectileClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FVector MuzzlePosition = GetMuzzleLocation()->GetComponentLocation();
			FRotator MuzzleRotation = GetMuzzleLocation()->GetComponentRotation();

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			AProjectile* SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzlePosition, MuzzleRotation, SpawnParams);
			if (SpawnedProjectile)
			{
				if (ProjectileSpeed > 0.f)
				{
					
					UProjectileMovementComponent* ProjectileMovementComponent = SpawnedProjectile->GetProjectileMovement();
					ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
				}
				
				FVector LaunchDirection = MuzzleRotation.Vector();
				SpawnedProjectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

void AProjectileWeapons::PlayWeaponEffect()
{
	Super::PlayWeaponEffect();

	USceneComponent* MuzzleComponent = GetMuzzleLocation();
	if (MuzzleComponent)
	{
		if (MuzzleFlashEffect)
		{
			FVector EffectLocation = MuzzleComponent->GetComponentLocation() + MuzzleComponent->GetForwardVector();
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashEffect, EffectLocation, MuzzleComponent->GetComponentRotation());
			UE_LOG(LogTemp, Warning, TEXT("Muzzle Flash effect triggered!"));
		}

		if (RearMuzzleEffect)
		{
			FVector EffectLocation = RearMuzzleLocation->GetComponentLocation() + RearMuzzleLocation->GetForwardVector();

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RearMuzzleEffect, EffectLocation, RearMuzzleLocation->GetComponentRotation());
			UE_LOG(LogTemp, Warning, TEXT("REAR Muzzle Flash effect triggered!"));

		}
		
		if (TracerEffect)
		{
			FVector EffectLocation = MuzzleComponent->GetComponentLocation();
			FVector MuzzleDirection = MuzzleComponent->GetForwardVector();
			
			FVector TraceEnd = EffectLocation + (MuzzleDirection * TrailEffectDistance);

			FHitResult HitResult;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EffectLocation, TraceEnd, ECC_Visibility, QueryParams);

			if (bHit)
			{
				TraceEnd = HitResult.ImpactPoint;
			}
			
			FRotator TracerRotation = (TraceEnd - EffectLocation).Rotation();
			
			UParticleSystemComponent* Tracer = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, EffectLocation, TracerRotation);
			if (Tracer)
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

void AProjectileWeapons::ApplyRecoilEffect()
{
	if (!RecoilSettings.bCanApplyRecoil || !GetWeaponMesh()) return;
	
	Super::ApplyRecoilEffect();

	float ExtraRecoilAmount = RecoilSettings.BaseRecoilAmount * 1.5f;
	float ExtraPitch = FMath::FRandRange(-ExtraRecoilAmount, -ExtraRecoilAmount * 0.8f);
	float ExtraYaw = FMath::FRandRange(-ExtraRecoilAmount * 0.1f, ExtraRecoilAmount * 0.1f);

	FVector OriginalLocation = GetWeaponMesh()->GetRelativeLocation();
	FRotator OriginalControlRotation;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		OriginalControlRotation = PlayerController->GetControlRotation();

		// Geri Tepme 
		GetWeaponMesh()->SetRelativeLocation(OriginalLocation + FVector(-ExtraRecoilAmount, ExtraYaw, 0.0f));
		FRotator NewControlRotation = OriginalControlRotation;
		NewControlRotation.Pitch += ExtraPitch;
		NewControlRotation.Yaw += ExtraYaw;
		PlayerController->SetControlRotation(NewControlRotation);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [PlayerController, OriginalControlRotation]()
		{
			if (PlayerController)
			{
				PlayerController->SetControlRotation(OriginalControlRotation);
			}
		}, 0.1f, false);
	}

	FTimerHandle WeaponRecoilTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(WeaponRecoilTimerHandle, [this, OriginalLocation]()
	{
		if (GetWeaponMesh())
		{
			GetWeaponMesh()->SetRelativeLocation(OriginalLocation);
		}
	}, 0.1f, false);


	RecoilSettings.bCanApplyRecoil = false;
	GetWorld()->GetTimerManager().SetTimer(RecoilSettings.RecoilCooldownTimerHandle, this, &AWeaponBase::ResetRecoil, RecoilSettings.RecoilCooldown, false);
}
