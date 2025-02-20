// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnCombatComponent.h"
#include "Camera/CameraShakeBase.h"
#include "SurvivalCharacterCombatComponent.generated.h"

class UCharacterGameplayAbility;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatHitTargetDelegate, const AActor*, HitActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API USurvivalCharacterCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	USurvivalCharacterCombatComponent();

	FOnCombatHitTargetDelegate OnCombatHitTargetDelegate;

	UFUNCTION(BlueprintCallable, Category = "Survival|Combat")
	void ToggleFootCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);
	
	UFUNCTION(BlueprintCallable, Category = "Survival|Combat")
	float GetCharacterCurrentEquipWeaponDamageAtLevel(float InLevel) const;

	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;

};
