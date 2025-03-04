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


UENUM(BlueprintType)
enum class EFireMode : uint8
{
	SingleShot     UMETA(DisplayName = "Single Shot"),
	BurstShot      UMETA(DisplayName = "Burst Shot"),
	AutomaticShot  UMETA(DisplayName = "Automatic Shot")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UFireModeBaseComponent : public UActorComponent, public IFire
{
	GENERATED_BODY()

public:
	UFireModeBaseComponent();
	
protected:
	virtual void BeginPlay() override;

	UCharacterAbilitySystemComponent* GetCharacterAbilitySystemComponent() const;

	virtual void ResetFire();
	virtual void LoopModeFire();

	FGameplayTag FireTag;

	TWeakObjectPtr<URaycastWeaponData> RaycastWeaponDataPtr;
	TWeakObjectPtr<ARaycastWeapons> OwnerWeaponPtr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Fire Mode")
	EFireMode FireModeType;
public:
	virtual void Fire() override;
	virtual void EndFire() override;

	FORCEINLINE EFireMode GetFireModeType() const { return FireModeType; }
};
