// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Survival/WeaponPickupSystem/Interfaces/WeaponUIHandler.h"
#include "WeaponUIHandlerBase.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UWeaponUIHandlerBase : public UActorComponent, public IWeaponUIHandler
{
	GENERATED_BODY()

public:
	UWeaponUIHandlerBase();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeCallbacks(AWeaponBase* Weapon, ASurvivalCharacter* PlayerCharacter) override;
	virtual void SetupUI(AWeaponBase* Weapon) override;
	virtual void UpdateUI(AWeaponBase* Weapon) override;
	virtual void RemoveUI(AWeaponBase* Weapon) override;
	
	
};
