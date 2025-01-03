// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Survival/WeaponPickupSystem/CharacterBase/SurvivalCharacterBase.h"
#include "EnemyBase.generated.h"

class USelectedWidgetComponent;
class ULockedWidgetComponent;

UCLASS()
class SURVIVAL_API AEnemyBase : public ASurvivalCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyBase();

	FORCEINLINE ULockedWidgetComponent* GetLockedWidgetComponent() const { return LockedWidgetComponent; }
	FORCEINLINE USelectedWidgetComponent* GetSelectedWidgetComponent() const { return SelectedWidgetComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Components | Lockon")
	ULockedWidgetComponent* LockedWidgetComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components | Lockon")
	USelectedWidgetComponent* SelectedWidgetComponent;
};
