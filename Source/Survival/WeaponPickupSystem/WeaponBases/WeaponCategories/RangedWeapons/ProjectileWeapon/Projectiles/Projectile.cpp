// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Survival/WeaponPickupSystem/Data/ProjectileDataAssets/ProjectileData.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponCategories/RangedWeapons/ProjectileWeapons.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponComponents/WeaponEffectManagerComponent/ProjectileEffectManagerComponent.h"


AProjectile::AProjectile()
{
	// Collision Component
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetBoxExtent(FVector(15.0f, 3.0f, 3.0f));
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	RootComponent = CollisionComponent;
	//DENEME
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // Hem fizik hem de sorgu etkin
	CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel1); // Özel bir kanal veya WorldDynamic
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore); // Varsayılan olarak tüm kanalları yoksay
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // Duvarlarla çarpışmayı etkinleştir
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block); // Dinamik objelerle çarpışmayı etkinleştir
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); 

	// Projectile Mesh Component
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(CollisionComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Mesh has no collision; handled by CollisionComponent

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

void AProjectile::ProjectileInit(const AProjectileWeapons* ProjectileWeapon)
{
	if (UProjectileEffectManagerComponent* EffectManager = ProjectileWeapon->GetEffectManagerComponent())
	{
		EffectManager->InitializeProjectile(this);
	}
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileData->ProjectileSettings.InitialSpeed;
}


void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ProjectileData->ExplosiveSettings.bIsExplosive)
	{
		OnExplosion.Broadcast();
	}
	else
	{
		if (OtherActor && OtherActor != this && OtherActor != GetOwner())
		{
			UGameplayStatics::ApplyDamage(
				OtherActor,
				ProjectileData->ProjectileSettings.DamageAmount,
				GetInstigatorController(),
				this,
				nullptr);
			Destroy();
		}
	}
}


