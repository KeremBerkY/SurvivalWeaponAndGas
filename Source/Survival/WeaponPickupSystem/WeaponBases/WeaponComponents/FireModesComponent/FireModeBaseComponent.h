// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Survival/WeaponPickupSystem/Interfaces/Fire.h"
#include "FireModeBaseComponent.generated.h"


class ASurvivalCharacter;
class ARaycastWeapons;
class URaycastWeaponData;
class UCharacterAbilitySystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UFireModeBaseComponent : public UActorComponent, public IFire
{
	GENERATED_BODY()

public:
	UFireModeBaseComponent();

protected:
	virtual void BeginPlay() override;

	UCharacterAbilitySystemComponent* GetCharacterAbilitySystemComponent() const;

	FGameplayTag FireTag;

	// TWeakObjectPtr<URaycastWeaponData> RaycastWeaponDataPtr;
	// TWeakObjectPtr<ARaycastWeapons> OwnerWeaponPtr;
	URaycastWeaponData* RaycastWeaponDataPtr;
	ARaycastWeapons* OwnerWeaponPtr;

public:
	virtual void Fire() override;
	virtual void EndFire() override;
};
