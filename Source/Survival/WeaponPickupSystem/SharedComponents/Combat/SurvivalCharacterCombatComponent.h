// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnCombatComponent.h"
#include "SurvivalCharacterCombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVAL_API USurvivalCharacterCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	USurvivalCharacterCombatComponent();

	UFUNCTION(BlueprintCallable, Category = "Survival|Combat")
	void ToggleFootCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);
	
};
