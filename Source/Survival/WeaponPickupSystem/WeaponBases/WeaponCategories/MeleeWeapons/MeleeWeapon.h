// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/WeaponBases/WeaponBase.h"
#include "MeleeWeapon.generated.h"

class UBoxComponent;

DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*)

UCLASS()
class SURVIVAL_API AMeleeWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	AMeleeWeapon();
	
	FOnTargetInteractedDelegate OnWeaponHitTarget;
	FOnTargetInteractedDelegate OnWeaponPulledFromTarget;

	UFUNCTION()
	void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
