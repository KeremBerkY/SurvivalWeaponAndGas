// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class AProjectileWeapons;
class UProjectileData;
class UBoxComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;


// USTRUCT(BlueprintType)
// struct FProjectileSet
// {
// 	GENERATED_BODY()
//
// 	FProjectileSet()
// 		: InitialSpeed(0.f), DamageAmount(0.f), HitEffect(nullptr) {}
//
// 	// Movement speed of the projectile
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
// 	float InitialSpeed;
//
// 	// Damage amount dealt by the projectile
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
// 	float DamageAmount;
//
// 	// Visual effect to spawn upon impact
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
// 	UParticleSystem* HitEffect;
// };
//
// USTRUCT(BlueprintType)
// struct FExplosiveSet
// {
// 	GENERATED_BODY()
//
// 	FExplosiveSet()
// 		: ExplosionRadius(0.f), ExplosionDelay(0.f), ExplosiveDamage(0.f), bIsExplosive(false) {}
//
// 	// Explosion radius for explosive projectiles
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
// 	float ExplosionRadius;
//
// 	// Delay before explosion for delayed effects
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
// 	float ExplosionDelay;
//
// 	// Damage dealt by the explosion
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
// 	float ExplosiveDamage;
//
// 	// Is this projectile explosive
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
// 	bool bIsExplosive;
// };

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileInitialize, AProjectile*, Projectile); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExplosion); 

UCLASS()
class SURVIVAL_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();
	
	FOnExplosion OnExplosion;
	// FOnProjectileInitialize OnProjectileInitialize;

	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	FORCEINLINE UProjectileData* GetProjectileData() const { return ProjectileData; }
	
	void FireInDirection(const FVector& ShootDirection);

	void ProjectileInit(const AProjectileWeapons* ProjectileWeapon);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(EditAnywhere, Category = "Projectile Data")
	TObjectPtr<UProjectileData> ProjectileData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

private:
	AProjectileWeapons* OwnerWeapon;

};
