// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/CharacterBase/SurvivalCharacterBase.h"
#include "EnemyBase.generated.h"

UCLASS()
class SURVIVAL_API AEnemyBase : public ASurvivalCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	
};
