// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_EnemyBase.h"
#include "BTTask_ToggleStrafingState.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API UBTTask_ToggleStrafingState : public UBTTask_EnemyBase
{
	GENERATED_BODY()

public:
	UBTTask_ToggleStrafingState();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strafing")
	bool bShouldEnable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strafing")
	bool bShouldChangeMaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strafing")
	float StrafingWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strafing")
	FBlackboardKeySelector  InDefaultMaxWalkSpeedKey;
	
protected:
	virtual void OnEnemyExecuteTask(UBehaviorTreeComponent& OwnerComp) override;

};
