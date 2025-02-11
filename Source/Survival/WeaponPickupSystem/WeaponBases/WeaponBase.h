// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Survival/WeaponPickupSystem/UserInterface/Crosshair/CrosshairBaseWidget.h"
#include "WeaponBase.generated.h"

class UBoxComponent;
class UWeaponUIHandlerBase;
class UWeaponData;
class UImage;
class IFireMode;
class AWeaponPickup;
class ASurvivalCharacter;

UCLASS()
class SURVIVAL_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

	UFUNCTION()
	virtual void Attack(); // TODO: Melee Weaponlar da geleceği için ileride bunu Attack() diye çevir RangedWeapon sınıfında bu Attack(){ FireHandle(); } çağıracak.
	virtual void EndAttack();

protected:
	UFUNCTION()
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void PlayWeaponEffect();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data Asset")
	TObjectPtr<UWeaponData> WeaponBaseDataAsset;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Effects")	
	UParticleSystem* BaseEffect;
	
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	UBoxComponent* WeaponCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	UBoxComponent* WeaponHandleCollisionBox;
private:

	UPROPERTY()
	ASurvivalCharacter* OwningCharacter;
	UPROPERTY()
	UWeaponUIHandlerBase* WeaponUIHandlerBase;

	UPROPERTY()
	bool bIsAttackCooldownActive;

public:

	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE TObjectPtr<UWeaponData> GetWeaponDataAsset() const { return  WeaponBaseDataAsset.Get(); }
	FORCEINLINE ASurvivalCharacter* GetOwningCharacter() const { return OwningCharacter; }
	FORCEINLINE void SetOwningCharacter(ASurvivalCharacter* PlayerCharacter) { OwningCharacter = PlayerCharacter; }
	FORCEINLINE UWeaponUIHandlerBase* GetWeaponUIHandlerBase() const { return WeaponUIHandlerBase; }
	FORCEINLINE bool GetAttackCooldownActive() const { return bIsAttackCooldownActive; }
	FORCEINLINE void SetAttackCooldownActive(const bool AttackCooldown) { bIsAttackCooldownActive = AttackCooldown; }
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox; }	
	FORCEINLINE UBoxComponent* GetWeaponHandleCollisionBox() const { return WeaponHandleCollisionBox; }	

};
