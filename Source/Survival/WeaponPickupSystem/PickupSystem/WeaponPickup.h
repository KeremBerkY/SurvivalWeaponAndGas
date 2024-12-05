// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "WeaponBases/WeaponBase.h"
#include "WeaponPickup.generated.h"

class AWeaponBase;


UCLASS()
class SURVIVAL_API AWeaponPickup : public ABasePickup
{
	GENERATED_BODY()

public:
	AWeaponPickup();

	FORCEINLINE TSubclassOf<AWeaponBase> GetWeaponClass() const { return WeaponClass; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(AActor* Actor) override;

private:
	
	UPROPERTY(EditAnywhere, Category = "Weapon");
	TSubclassOf<AWeaponBase> WeaponClass;

	UPROPERTY(EditAnywhere, Category  = "Weapon")
	float DropDistance = 200.f;
	
};
