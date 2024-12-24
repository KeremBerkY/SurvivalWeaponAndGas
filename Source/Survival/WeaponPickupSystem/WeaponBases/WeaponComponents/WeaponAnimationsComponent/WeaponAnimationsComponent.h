// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponAnimationsComponent.generated.h"


class URaycastWeaponData;
class ARaycastWeapons;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UWeaponAnimationsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponAnimationsComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void InitializeAnims();
	UFUNCTION()
	void PlayAttackMontage();

private:
	ARaycastWeapons* Weapon;

	URaycastWeaponData* RaycastWeaponData;
};
