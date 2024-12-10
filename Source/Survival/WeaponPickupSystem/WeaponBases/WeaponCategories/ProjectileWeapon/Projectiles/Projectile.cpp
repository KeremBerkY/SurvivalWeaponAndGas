// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AProjectile::AProjectile()
{
	// Collision Component
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetBoxExtent(FVector(15.0f, 3.0f, 3.0f));
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	RootComponent = CollisionComponent;

	// Projectile Mesh Component
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(CollisionComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Mesh has no collision; handled by CollisionComponent

	// Projectile Movement Component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	// ProjectileMovement->InitialSpeed = ProjectileSettings.InitialSpeed;
	// ProjectileMovement->MaxSpeed = ProjectileSettings.InitialSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true; // Keeps the projectile pointed in the direction of travel
	ProjectileMovement->bShouldBounce = false;
	
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileSettings.InitialSpeed;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (ExplosiveSettings.bIsExplosive)
	{
		HandleExplosion();
	}
	else
	{
		if (OtherActor && OtherActor != this && OtherActor != GetOwner())
		{
			UGameplayStatics::ApplyDamage(
				OtherActor,
				ProjectileSettings.DamageAmount,
				GetInstigatorController(),
				this,
				nullptr);
			Destroy();
		}
	}
}

void AProjectile::HandleExplosion()
{
	if (ExplosiveSettings.ExplosionRadius > 0.f)
	{
		UGameplayStatics::ApplyRadialDamage(
			GetWorld(),
			ExplosiveSettings.ExplosiveDamage,
			GetActorLocation(),
			ExplosiveSettings.ExplosionRadius,
			nullptr,
			TArray<AActor*>(),
			this,
			GetInstigatorController(),
			true
			);

		Destroy();
	}
}

