// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/SharedComponents/PawnExtensionComponentBase.h"
#include "PawnCombatComponent.generated.h"

class AWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UPawnCombatComponent();

	UFUNCTION()
	AWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Survival|Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);

	UFUNCTION()
	virtual void OnHitTargetActor(AActor* HitActor);
	UFUNCTION()
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

protected:
	UPROPERTY()
	TArray<AActor*> OverlappedActors;

};
