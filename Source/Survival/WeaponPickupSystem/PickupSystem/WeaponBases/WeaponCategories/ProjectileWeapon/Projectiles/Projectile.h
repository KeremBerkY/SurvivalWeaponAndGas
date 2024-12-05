// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;


USTRUCT(BlueprintType)
struct FProjectileSettings
{
	GENERATED_BODY()

	FProjectileSettings()
		: InitialSpeed(0.f), DamageAmount(0.f), HitEffect(nullptr) {}

	// Movement speed of the projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float InitialSpeed;

	// Damage amount dealt by the projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAmount;

	// Visual effect to spawn upon impact
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* HitEffect;
};

USTRUCT(BlueprintType)
struct FExplosiveSettings
{
	GENERATED_BODY()

	FExplosiveSettings()
		: ExplosionRadius(0.f), ExplosionDelay(0.f), ExplosiveDamage(0.f), bIsExplosive(false) {}

	// Explosion radius for explosive projectiles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosionRadius;

	// Delay before explosion for delayed effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosionDelay;

	// Damage dealt by the explosion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosiveDamage;

	// Is this projectile explosive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	bool bIsExplosive;
};
UCLASS()
class SURVIVAL_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	
	void FireInDirection(const FVector& ShootDirection);

	
protected:

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void HandleExplosion();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FProjectileSettings ProjectileSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FExplosiveSettings ExplosiveSettings;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

private:
};
