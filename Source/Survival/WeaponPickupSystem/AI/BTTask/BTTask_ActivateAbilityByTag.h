// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_EnemyBase.h"
#include "BTTask_ActivateAbilityByTag.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UBTTask_ActivateAbilityByTag : public UBTTask_EnemyBase
{
	GENERATED_BODY()

public:
	virtual void OnEnemyExecuteTask(UBehaviorTreeComponent& OwnerComp) override;

	UPROPERTY(EditAnywhere)
	FGameplayTag AbilityTagToActivate;
	
};
